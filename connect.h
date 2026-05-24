#warning y9INC: This code has not been tested yet
#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>

/**
 * @file connect.h
 * @brief Windows 共有メモリ（名前付きファイルマッピング）通信クラス
 *
 * Windows の CreateFileMapping / OpenFileMapping API を使用して
 * プロセス間共有メモリを確立し、任意の型のデータを読み書きします。
 *
 * @warning このコードはまだテストされていません (y9INC: This code has not been tested yet)
 */

/**
 * @class PConnect
 * @brief 名前付き共有メモリを介してプロセス間通信を行うテンプレートクラス
 * @tparam T 共有するデータの型
 *
 * 2 つのコンストラクタで「作成モード」と「オープンモード」を区別します：
 * - 作成モード: `PConnect(name, CREATEMODE___IMiNaSi)` → 共有メモリを新規作成
 * - オープンモード: `PConnect(name)` → 既存の共有メモリに接続
 *
 * @code
 * // プロセスA（作成側）
 * PConnect<int> server("MySharedMem", 0);
 * server.write(42);
 *
 * // プロセスB（接続側）
 * PConnect<int> client("MySharedMem");
 * int val = client.read(); // 42
 * @endcode
 */
template<typename T>
class PConnect {
private:
	HANDLE hMapFile;       ///< 共有メモリのハンドル
	T* pBuf;               ///< マッピングされたメモリ（テンプレート型ポインタ）
	std::string name;      ///< 共有メモリの名前（"Global\\" プレフィックス付き）

public:
	/**
	 * @brief 共有メモリを新規作成するコンストラクタ（作成モード）
	 * @param memName              共有メモリの名前（"Global\\" が自動付加される）
	 * @param CREATEMODE___IMiNaSi 作成モードであることを示すダミー引数（値は使用しない）
	 * @throws std::runtime_error 共有メモリの作成またはマッピングに失敗した場合
	 */
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

		if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) {
			DWORD errorCode = GetLastError();
			std::cerr << "共有メモリ作成エラー: " << errorCode << " (" << PConnect::getLastErrorAsString() << ")" << std::endl;

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

	/**
	 * @brief 既存の共有メモリに接続するコンストラクタ（オープンモード）
	 * @param memName 接続する共有メモリの名前（"Global\\" が自動付加される）
	 * @throws std::runtime_error 共有メモリのオープンまたはマッピングに失敗した場合
	 */
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

	/**
	 * @brief 共有メモリにデータを書き込む
	 * @param data 書き込むデータ（型 T）
	 * @throws std::runtime_error マッピングポインタが無効な場合
	 */
	void write(const T& data) {
		if (!pBuf) {
			throw std::runtime_error("書き込み対象が無効です");
		}
		*pBuf = data;
	}

	/**
	 * @brief 共有メモリからデータを読み込む
	 * @return 共有メモリの現在値（型 T のコピー）
	 * @throws std::runtime_error マッピングポインタが無効な場合
	 */
	T read() const {
		if (!pBuf) {
			throw std::runtime_error("読み込み対象が無効です");
		}
		return *pBuf;
	}

	/**
	 * @brief デストラクタ。マッピングとハンドルを解放する
	 */
	~PConnect() {
		if (pBuf) {
			UnmapViewOfFile(pBuf);
		}
		if (hMapFile) {
			CloseHandle(hMapFile);
		}
	}
	
private:
	/**
	 * @brief 最後の Windows エラーコードのメッセージ文字列を取得する
	 * @return エラーメッセージ文字列。エラーがない場合は "エラーは発生していません"
	 */
	static std::string getLastErrorAsString() {
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0) {
			return "エラーは発生していません";
		}

		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorMessageID,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer,
			0,
			NULL);

		std::string message(messageBuffer, size);

		LocalFree(messageBuffer);

		return message;
	}
};
