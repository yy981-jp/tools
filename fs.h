#pragma once
#include <string>
#include <shobjidl.h>
#include <shlguid.h>
#include <objbase.h>

#include <yy981/proc.h>

/**
 * @file fs.h
 * @brief ファイルシステム補助ユーティリティ（Windows）
 *
 * Windows Shell API を用いた .lnk ショートカットの解決と、
 * ショートカット対応のアプリ起動機能を提供します。
 */

/**
 * @namespace yfs
 * @brief yy981 ファイルシステムユーティリティ用名前空間
 */
namespace yfs {

/**
 * @brief Windows ショートカット (.lnk) ファイルのリンク先パスを解決する
 * @param lnkPath 解決する .lnk ファイルのパス（std::string）
 * @return リンク先の実ファイルパス文字列
 * @throws std::runtime_error .lnk の解決に失敗した場合
 * @note COM (CoInitialize/CoUninitialize) を内部で初期化・解放します
 */
std::string resolveShortcut(const std::string& lnkPath) {
	CoInitialize(NULL);

	IShellLinkA* pShellLink = nullptr;
	IPersistFile* pPersistFile = nullptr;
	char targetPath[MAX_PATH] = {};

	std::string ret;

	if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (void**)&pShellLink))) {
		if (SUCCEEDED(pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile))) {
			std::wstring wpath(lnkPath.begin(), lnkPath.end());
			if (SUCCEEDED(pPersistFile->Load(wpath.c_str(), STGM_READ))) {
				if (SUCCEEDED(pShellLink->GetPath(targetPath, MAX_PATH, NULL, SLGP_RAWPATH))) {
					ret = std::string(targetPath);
				}
			}
			pPersistFile->Release();
		}
		pShellLink->Release();
	}

	CoUninitialize();

	if (ret.empty()) {
		throw std::runtime_error("yy981/fs::resolveShortcut(): .lnkの解決に失敗");
	}
	return ret;
}

/**
 * @brief アプリケーションを起動する（.lnk ショートカット対応）
 * @param app  起動するアプリケーションのパス。.lnk 拡張子の場合は自動的にリンク先を解決します
 * @param arg  コマンドライン引数（デフォルト: 空文字列）
 * @param wait プロセスの終了を待つか（デフォルト: false）
 * @param cd   起動時のカレントディレクトリ（デフォルト: 空文字列 = 親プロセスと同じ）
 * @return wait=true の場合は起動したプロセスの終了コード、wait=false の場合は 0
 * @throws std::runtime_error 起動失敗または .lnk 解決失敗時
 */
int start(const std::string app, std::string arg = "", const bool wait = false, const std::string& cd = "") {
	if (app.ends_with(".lnk")) return proc::start(resolveShortcut(app),arg,wait,cd);
	return proc::start(app,arg,wait,cd);
}

}
