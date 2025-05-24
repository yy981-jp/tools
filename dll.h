#ifdef BUILD
    #define DLL extern "C" __declspec(dllexport)
#else
    // #define DLL __declspec(dllimport)
	#pragma once
	#include <windows.h>
	#include <string>
	#include <cstdint>
	#include <stdexcept>
	// 関数テンプレートオブジェクト
	template<typename R, typename... Args>
	class DLL {
	public:
		DLL(HMODULE hModule, std::string functionName): func(nullptr), hModule(hModule) {functionName = functionName.c_str();}

		R operator()(Args... args) {
			if (!func) {
				func = reinterpret_cast<R(*)(Args...)>(GetProcAddress(hModule, functionName));
				if (!func) {
					throw std::runtime_error("Failed to load function: " + std::string(functionName));
				}
			}
			return func(std::forward<Args>(args)...);
		}
		
	private:
		R(*func)(Args...); // 関数ポインタ
		HMODULE hModule;   // DLLモジュール
		const char* functionName; // 関数名
	};
	
	// DLLモジュール解放
	static void cleanDLL(HMODULE& dll) {
		if (dll) {
			FreeLibrary(dll);
			dll = nullptr;
		}
	}
#endif