#pragma once
#include <string>
#include <cstdlib>
#include <yy981/string.h>

#ifdef _WIN32  // Windows
	#include <windows.h>
#else		   // UNIX系
	#include <unistd.h>
#endif

/**
 * @file env.h
 * @brief 環境変数取得・設定ユーティリティ
 *
 * クロスプラットフォーム（Windows / UNIX 系）で環境変数を取得・設定する関数を提供します。
 * Windows ではレジストリへの恒久的な書き込みにも対応しています。
 */

/**
 * @brief 環境変数の値を取得する（簡易版）
 * @param target 取得する環境変数名（例: "PATH"）
 * @param slash  true の場合、バックスラッシュをスラッシュに変換する（デフォルト: true）
 * @return 環境変数の値文字列。slash=true の場合は \ を / に置換済み
 */
inline std::string getEnv(const std::string& target, const bool slash = true) {
	std::string result = std::getenv(target.c_str());
	if (!slash) return result;
	return st::replace(result,"\\","/");
}

/* 機能しない
std::string getEnv(const std::string& target, const std::string& add = "") {
	const char* result_raw = std::getenv(target.c_str());
	if (!result_raw) return std::string();
	const std::string result{result_raw};
	bool s(add.contains("/")), bs(add.contains("\\"));
	if ((!s&&!bs) || (s&&bs) || s) return st::replace(result,"\\","/") + add;
	else if (bs) return st::replace(result,"/","\\") + add;
	else throw std::runtime_error("getEnv(): スラッシュ-バックスラッシュ 判定エラー");
}

std::string getEnv(const std::string& target, const bool backSlash) {
	std::string result;
	if (backSlash) result = getEnv(target,"\\");
	else result = getEnv(target,"/");
	return result.substr(0, result.size()-1);
}
*/

/**
 * @brief 環境変数をプロセス内で設定する（現在のプロセスのみ有効）
 * @param name          設定する環境変数名
 * @param value         設定する値
 * @param overrideExist 既存の変数を上書きするか（現在の実装では常に上書き）
 * @return 設定に成功した場合 true、失敗した場合 false
 */
inline bool setEnv(const char* name, const char* value, bool overrideExist = true) {
#ifdef _WIN32
	if (_putenv_s(name, value)) return false; else return true;
#else
	// UNIX系での設定
	if (setenv(name, value, 1)) return false; else return true;
#endif
}

/**
 * @brief 環境変数を Windows レジストリ (HKCU\\Environment) に恒久的に設定する
 * @param varName  設定する環境変数名
 * @param varValue 設定する値
 * @return 設定に成功した場合 true、失敗した場合 false
 * @note Windows 専用。設定後に WM_SETTINGCHANGE を送信し他のアプリに変更を通知します。
 * @warning UNIX 系では常に false を返し、コンパイル警告が発生します。
 */
inline bool setEnvEx(const std::string& varName, const std::string& varValue) {
#ifdef _WIN32
	HKEY hKey;
	LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0, KEY_SET_VALUE, &hKey);
	if (result != ERROR_SUCCESS) {
		// std::cerr << "レジストリキーを開けませんでした\n";
		return false;
	}

	result = RegSetValueExA(hKey, varName.c_str(), 0, REG_SZ, (const BYTE*)varValue.c_str(), varValue.size() + 1);
	RegCloseKey(hKey);

	if (result != ERROR_SUCCESS) {
		// std::cerr << "環境変数の設定に失敗しました\n";
		return false;
	}

	// 変更を即座に反映させる
	SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000, nullptr);

	return true;
#else
	#warning
	return false;
#endif
}
