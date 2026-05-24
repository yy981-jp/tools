#ifdef BUILD
    #define DLL extern "C" __declspec(dllexport)
#else
    // #define DLL __declspec(dllimport)
	#pragma once
	#include <windows.h>
	#include <string>
	#include <cstdint>
	#include <stdexcept>

/**
 * @file dll.h
 * @brief DLL 動的ロードユーティリティ
 *
 * - `BUILD` マクロが定義されている場合: `DLL` は `extern "C" __declspec(dllexport)` に展開されます。
 * - 定義されていない場合: DLL から関数を遅延バインディングで呼び出す `DLL<R, Args...>` テンプレートクラスが提供されます。
 */

	/**
	 * @class DLL
	 * @brief DLL から関数を遅延バインディングで呼び出すファンクターテンプレートクラス
	 * @tparam R    関数の戻り値型
	 * @tparam Args 関数の引数型（パック）
	 *
	 * 初回呼び出し時に GetProcAddress で関数ポインタを解決し、その後はキャッシュした
	 * ポインタを使って呼び出します。
	 *
	 * @code
	 * HMODULE hMod = LoadLibrary("example.dll");
	 * DLL<int, const char*> myFunc(hMod, "MyFunction");
	 * int result = myFunc("hello");
	 * @endcode
	 */
	template<typename R, typename... Args>
	class DLL {
	public:
		/**
		 * @brief コンストラクタ
		 * @param hModule      関数を検索する DLL モジュールハンドル
		 * @param functionName 取得する関数名
		 */
		DLL(HMODULE hModule, std::string functionName): func(nullptr), hModule(hModule) {functionName = functionName.c_str();}

		/**
		 * @brief 関数を呼び出す（初回は GetProcAddress で解決）
		 * @param args 関数に渡す引数
		 * @return 関数の戻り値
		 * @throws std::runtime_error 関数が見つからない場合
		 */
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
		R(*func)(Args...); ///< 解決済み関数ポインタ
		HMODULE hModule;   ///< DLL モジュールハンドル
		const char* functionName; ///< 関数名
	};
	
	/**
	 * @brief DLL モジュールを解放する
	 * @param dll 解放する HMODULE 変数への参照（解放後 nullptr にセットされる）
	 */
	static void cleanDLL(HMODULE& dll) {
		if (dll) {
			FreeLibrary(dll);
			dll = nullptr;
		}
	}
#endif
