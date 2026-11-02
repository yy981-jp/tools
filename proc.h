#pragma once
#include <windows.h>
#include <limits>
#include <stdexcept>
#include <string>

/**
 * @file proc.h
 * @brief プロセス起動ユーティリティ（Windows）
 *
 * Windows API の CreateProcess を使用して外部プロセスを起動します。
 * UNICODE ビルドと ANSI ビルドの両方に対応しています。
 */

namespace proc {
#ifdef UNICODE
	/**
	 * @brief UTF-8 文字列を wstring に変換する（UNICODE ビルド専用）
	 * @param u8 変換元の UTF-8 文字列
	 * @return 変換後の wstring
	 */
	inline std::wstring to_wstring(const std::string& u8) {
		if (u8.empty()) return {};
		if (u8.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
			throw std::length_error("yy981/proc.h::to_wstring(): 入力が長すぎます");
		}

		const int inputSize = static_cast<int>(u8.size());
		const int outputSize = MultiByteToWideChar(
			CP_UTF8, MB_ERR_INVALID_CHARS, u8.data(), inputSize, nullptr, 0
		);
		if (outputSize == 0) {
			throw std::runtime_error(
				"yy981/proc.h::to_wstring(): UTF-8変換失敗 | GetLastError()=" +
				std::to_string(GetLastError())
			);
		}

		std::wstring result(static_cast<size_t>(outputSize), L'\\0');
		if (MultiByteToWideChar(
			CP_UTF8, MB_ERR_INVALID_CHARS, u8.data(), inputSize,
			result.data(), outputSize
		) != outputSize) {
			throw std::runtime_error(
				"yy981/proc.h::to_wstring(): UTF-8変換失敗 | GetLastError()=" +
				std::to_string(GetLastError())
			);
		}
		return result;
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
		const std::wstring application = to_wstring(app);
		std::wstring commandLine = to_wstring(app + " " + arg);
		const std::wstring workingDirectory = cd.empty() ? std::wstring{} : to_wstring(cd);

		STARTUPINFOW si{};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi{};

		if (!CreateProcessW(
			application.c_str(),
			commandLine.data(),
			nullptr,
			nullptr,
			FALSE,
			0,
			nullptr,
			workingDirectory.empty() ? nullptr : workingDirectory.c_str(),
			&si,
			&pi
		)) {
			throw std::runtime_error(
				"yy981/proc.h::start(): 起動失敗 | GetLastError()=" +
				std::to_string(GetLastError())
			);
		}

		if (wait) {
			WaitForSingleObject(pi.hProcess, INFINITE);
			DWORD exitCode = 0;
			const BOOL succeeded = GetExitCodeProcess(pi.hProcess, &exitCode);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			if (!succeeded) {
				throw std::runtime_error(
					"yy981/proc.h::start(): 返り値取得失敗 | GetLastError()=" +
					std::to_string(GetLastError())
				);
			}
			return static_cast<int>(exitCode);
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
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
		std::string commandLine = app + " " + arg;
		STARTUPINFOA si{};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi{};

		if (!CreateProcessA(
			app.c_str(),
			commandLine.data(),
			nullptr,
			nullptr,
			FALSE,
			0,
			nullptr,
			cd.empty() ? nullptr : cd.c_str(),
			&si,
			&pi
		)) {
			throw std::runtime_error(
				"yy981/proc.h::start(): 起動失敗 | GetLastError()=" +
				std::to_string(GetLastError())
			);
		}

		if (wait) {
			WaitForSingleObject(pi.hProcess, INFINITE);
			DWORD exitCode = 0;
			const BOOL succeeded = GetExitCodeProcess(pi.hProcess, &exitCode);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			if (!succeeded) {
				throw std::runtime_error(
					"yy981/proc.h::start(): 返り値取得失敗 | GetLastError()=" +
					std::to_string(GetLastError())
				);
			}
			return static_cast<int>(exitCode);
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return 0;
	}
#endif
}
