#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <boost/locale.hpp>

namespace proc {
#ifdef UNICODE
	std::wstring to_wstring(const std::string& u8) {
		return boost::locale::conv::to_utf<wchar_t>(u8, "UTF-8");
	}

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