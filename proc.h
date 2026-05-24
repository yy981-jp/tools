/**
 * @file proc.h
 * @brief プロセス起動機能をラッピング
 * @details Windows API (CreateProcess)を使用した外部プロセス起動
 * @author yy981
 * @version 1.0
 * @note Windows専用、UNICODE定義の有無で動作が異なる
 */
#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <boost/locale.hpp>

/**
 * @namespace proc
 * @brief プロセス操作関数の名前空間
 */
namespace proc {
#ifdef UNICODE
	/**
	 * @brief UTF-8文字列をワイド文字列に変換
	 * @param u8 UTF-8文字列
	 * @return ワイド文字列
	 * @note UNICODE定義時のみ有効
	 */
	inline std::wstring to_wstring(const std::string& u8) {
		return boost::locale::conv::to_utf<wchar_t>(u8, "UTF-8");
	}

	/**
	 * @brief 外部プロセスを起動（UNICODE対応）
	 * @param app アプリケーションパス
	 * @param arg コマンドライン引数
	 * @param wait プロセスの完了を待機するか
	 * @param cd カレントディレクトリ
	 * @return wait=trueの場合はプロセスの終了コード、wait=falseの場合は0
	 * @exception std::runtime_error プロセス起動に失敗した場合
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
	 * @brief 外部プロセスを起動（マルチバイト版）
	 * @param app アプリケーションパス
	 * @param arg コマンドライン引数
	 * @param wait プロセスの完了を待機するか
	 * @param cd カレントディレクトリ
	 * @return wait=trueの場合はプロセスの終了コード、wait=falseの場合は0
	 * @exception std::runtime_error プロセス起動に失敗した場合
	 * @note UNICODE未定義時のみ有効
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