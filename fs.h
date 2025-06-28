#pragma once
#include <string>
#include <shobjidl.h>
#include <shlguid.h>
#include <objbase.h>

#include <yy981/proc.h>

namespace yfs {



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

int start(const std::string app, std::string arg = "", const bool wait = false, const std::string& cd = "") {
	if (app.ends_with(".lnk")) return proc::start(resolveShortcut(app),arg,wait,cd);
	return proc::start(app,arg,wait,cd);
}



}