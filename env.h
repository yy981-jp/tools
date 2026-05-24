/**
 * @file env.h
 * @brief 環境変数の取得・設定機能を提供
 * @details Windows/Unix両対応の環境変数操作ユーティリティ
 * @author yy981
 * @version 1.0
 */
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
 * @brief 環境変数を取得（簡易版）
 * @param target 取得する環境変数名
 * @param slash スラッシュ変換フラグ（true: バックスラッシュをスラッシュに置換）
 * @return 環境変数の値
 * @details スラッシュ置換を有効にした場合、バックスラッシュをスラッシュに置換
 */
inline std::string getEnv(const std::string& target, const bool slash = true) {
	std::string result = std::getenv(target.c_str());
	if (!slash) return result;
	return st::replace(result,"\\","/");
}

/**
 * @brief 環境変数を設定（プロセス内）
 * @param name 環境変数名
 * @param value 設定する値
 * @param overrideExist 既存値の上書きフラグ（現在未使用）
 * @return true: 設定成功、false: 設定失敗
 * @details プロセス内でのみ有効（システム全体の環境変数は変更されない）
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
 * @brief 環境変数をシステムに設定（Windows拡張版）
 * @param varName 環境変数名
 * @param varValue 設定する値
 * @return true: 設定成功、false: 設定失敗
 * @details Windowsのレジストリに直接設定し、システム全体に反映（要管理者権限）
 * @note Windows専用、他のOS上では常にfalseを返す
 * @warning 管理者権限が必要です
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
