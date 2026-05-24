/**
 * @file fs.h
 * @brief ファイルシステム操作と外部アプリケーション起動機能を提供
 * @details ショートカット(.lnk)ファイルの解決とプロセス起動関数
 * @author yy981
 * @version 1.0
 * @note Windows専用 (COM初期化、IShellLink等を使用)
 */
#pragma once
#include <string>
#include <shobjidl.h>
#include <shlguid.h>
#include <objbase.h>

#include <yy981/proc.h>

/**
 * @namespace yfs
 * @brief ファイルシステム操作関数の名前空間
 */
namespace yfs {



/**
 * @brief ショートカット(.lnk)ファイルから実際のパスを取得
 * @param lnkPath ショートカットファイルのパス
 * @return 実際のターゲットパス
 * @exception std::runtime_error .lnkファイルの解決に失敗した場合
 * @details
 * Windowsのシェルリンク機能を使用してショートカットを解決します
 * COM初期化と解放は自動的に行われます
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
 * @brief アプリケーション(またはショートカット)を起動
 * @param app アプリケーションパス（.lnkの場合は自動で解決）
 * @param arg コマンドライン引数
 * @param wait 起動完了待機フラグ
 * @param cd カレントディレクトリ
 * @return プロセスの終了コード（wait=trueの場合）、またはコマンドラインシェルのerrno
 * @exception std::runtime_error プロセス起動に失敗した場合
 * @details
 * .lnkで終わるパスの場合は自動的にresolveShortcut()で解決してから起動
 */
int start(const std::string app, std::string arg = "", const bool wait = false, const std::string& cd = "") {
	if (app.ends_with(".lnk")) return proc::start(resolveShortcut(app),arg,wait,cd);
	return proc::start(app,arg,wait,cd);
}



}