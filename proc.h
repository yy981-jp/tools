#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <boost/locale.hpp>

/**
 * @file proc.h
 * @brief プロセス起動ユーティリティ（Windows）
 *
 * Windows API の CreateProcess を使用して外部プロセスを起動します。
 * UNICODE ビルドと ANSI ビルドの両方に対応しています。
 */

/**
 * @namespace proc
 * @brief プロセス操作用名前空間
 */
namespace proc {
#ifdef UNICODE
	/**
	 * @brief UTF-8 文字列を wstring に変換する（UNICODE ビルド専用）
	 * @param u8 変換元の UTF-8 文字列
	 * @return 変換後の wstring
	 */
	inline std::wstring to_wstring(const std::string& u8) {
		return boost::locale::conv::to_utf<wchar_t>(u8, "UTF-8");
	}

	/**
	 * @brief 外部アプリケーションを起動する（UNICODE ビルド版）
	 * @param app  起動するアプリケーションのパス（UTF-8 文字列）
	 * @param arg  コマンドライン引数（デフォルト: 空文字列）
	 * @param wait プロセスの終了を待つか（デフォルト: false）
	 * @param cd   起動時のカレントディレクトリ（デフォルト: 空文字列 = 親と同じ）
	 * @return wait=true の場合は終了コード、wait=false の場合は 0
	 * @throws std::runtime_error 起動失敗または終了コード取得失敗時
	 */
	inline int start(const std::string app, std::string arg = "", const bool wait = false, const std::string& cd = "") {
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		if (CreateProcess(
			to_wstring(app).c_str(),	// アプリのパス
			to_wstring(app + " " + arg).data(),   // コマンドライン引数（アプリ名含めない）
			NULL, NULL,	 // セキュリティ属性
			false,		  // 子プロセスにハンドルを継承させない
			0,			  // 作成フラグ（CREATE_NO_WINDOWとかも指定可能）
			NULL,		   // 環境変数（NULLなら親プロセスと同じ）
			(cd.empty()? NULL: to_wstring(cd).c_str()),	// カレントディレクトリ（NULLなら親と同じ）
			&si, &pi
		)) {
			if (wait) {
				WaitForSingleObject(pi.hProcess, INFINITE);
				DWORD exitCode;
				if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
					return exitCode;
				} else throw std::runtime_error("yy981/proc.h::start(): 返り値取得失敗");
			}
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		} else throw std::runtime_error("yy981/proc.h::start(): 起動失敗 | GetLastError()=" + std::to_string(GetLastError()));
		return 0;
	}

#else
	/**
	 * @brief 外部アプリケーションを起動する（ANSI ビルド版）
	 * @param app  起動するアプリケーションのパス
	 * @param arg  コマンドライン引数（デフォルト: 空文字列）
	 * @param wait プロセスの終了を待つか（デフォルト: false）
	 * @param cd   起動時のカレントディレクトリ（デフォルト: 空文字列 = 親と同じ）
	 * @return wait=true の場合は終了コード、wait=false の場合は 0
	 * @throws std::runtime_error 起動失敗または終了コード取得失敗時
	 */
	inline int start(const std::string app, std::string arg = "", const bool wait = false, const std::string& cd = "") {
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		if (CreateProcess(
			app.c_str(),	// アプリのパス
			(app + " " + arg).data(),   // コマンドライン引数（アプリ名含めない）
			NULL, NULL,	 // セキュリティ属性
			false,		  // 子プロセスにハンドルを継承させない
			0,			  // 作成フラグ（CREATE_NO_WINDOWとかも指定可能）
			NULL,		   // 環境変数（NULLなら親プロセスと同じ）
			(cd.empty()? NULL: cd.c_str()),	// カレントディレクトリ（NULLなら親と同じ）
			&si, &pi
		)) {
			if (wait) {
				WaitForSingleObject(pi.hProcess, INFINITE);
				DWORD exitCode;
				if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
					return exitCode;
				} else throw std::runtime_error("yy981/proc.h::start(): 返り値取得失敗");
			}
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		} else throw std::runtime_error("yy981/proc.h::start(): 起動失敗 | GetLastError()=" + std::to_string(GetLastError()));
		return 0;
	}

#endif
}
