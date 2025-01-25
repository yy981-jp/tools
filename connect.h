#warning y9INC: This code has not been tested yet
#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>


std::string getLastErrorAsString() {
	// エラーコードを取得
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return "エラーは発生していません"; // エラーがない場合
	}

	// エラーメッセージを格納するバッファ
	LPSTR messageBuffer = nullptr;

	// エラーメッセージを取得
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		NULL);

	std::string message(messageBuffer, size);

	// バッファを解放
	LocalFree(messageBuffer);

	return message;
}

template<typename T>
class PConnect {
private:
	HANDLE hMapFile;       // 共有メモリのハンドル
	T* pBuf;               // マッピングされたメモリ（テンプレート型ポインタ）
	std::string name;     // 共有メモリの名前

public:
	// コンストラクタ（作成モード）
	PConnect(const std::string& memName, int8_t CREATEMODE___IMiNaSi)
		: hMapFile(nullptr), pBuf(nullptr), name("Global\\"+memName) {
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,         // 物理ファイルを使わない
			NULL,                         // デフォルトのセキュリティ
			PAGE_READWRITE,               // 読み書き可能
			0,                            // サイズ（高位）
			sizeof(T),                    // サイズ（低位）
			name.c_str()                  // 共有メモリの名前
		);
		// std::cout << "DEBUG:::EM: " <<  << "\n";

		// if (!hMapFile) throw std::runtime_error("共有メモリの作成に失敗しました WinAPI: " + getLastErrorAsString());
		if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) {
			DWORD errorCode = GetLastError();
			std::cerr << "共有メモリ作成エラー: " << errorCode << " (" << getLastErrorAsString() << ")" << std::endl;

			if (errorCode == ERROR_ACCESS_DENIED) {
				std::cerr << "解決策: 管理者権限を使用するか、共有メモリ名を確認してください。" << std::endl;
			} else if (errorCode == ERROR_ALREADY_EXISTS) {
				std::cerr << "解決策: 既存の共有メモリに接続するか、異なる名前を使用してください。" << std::endl;
			} else {
				std::cerr << "詳細なエラー内容を確認してください。" << std::endl;
			}

			throw std::runtime_error("共有メモリの作成に失敗しました。");
	}

		// メモリをプロセスにマッピング
		pBuf = static_cast<T*>(MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, sizeof(T)));
		if (!pBuf) {
			CloseHandle(hMapFile);
			throw std::runtime_error("共有メモリのマッピングに失敗しました");
		}
	}

	// コンストラクタ（オープンモード）
	PConnect(const std::string& memName)
		: hMapFile(nullptr), pBuf(nullptr), name("Global\\"+memName) {
		hMapFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, name.c_str());
		if (!hMapFile) throw std::runtime_error("共有メモリのオープンに失敗しました");

		pBuf = static_cast<T*>(MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, sizeof(T)));
		if (!pBuf) {
			CloseHandle(hMapFile);
			throw std::runtime_error("共有メモリのマッピングに失敗しました");
		}
	}

	// データの書き込み
	void write(const T& data) {
		if (!pBuf) {
			throw std::runtime_error("書き込み対象が無効です");
		}
		*pBuf = data;
	}

	// データの読み込み
	T read() const {
		if (!pBuf) {
			throw std::runtime_error("読み込み対象が無効です");
		}
		return *pBuf;
	}

	~PConnect() {
		if (pBuf) {
			UnmapViewOfFile(pBuf);
		}
		if (hMapFile) {
			CloseHandle(hMapFile);
		}
	}
};