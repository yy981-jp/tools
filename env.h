#pragma once
#include <string>
#include <cstdlib>
#include <yy981/string.h>

#ifdef _WIN32  // Windows
	#include <windows.h>
#else		   // UNIX系
	#include <unistd.h>
#endif

// 簡易版
std::string getEnv(const std::string& target, const bool slash = true) {
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

bool setEnv(const char* name, const char* value, bool overrideExist = true) {
#ifdef _WIN32
	if (_putenv_s(name, value)) return false; else return true;
#else
	// UNIX系での設定
	if (setenv(name, value, 1)) return false; else return true;
#endif
}

bool setEnvEx(const std::string& varName, const std::string& varValue) {
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
