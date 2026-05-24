/**
 * @file dll.h
 * @brief DLL(動的ライブラリ)の読み込みと関数呼び出し機能を提供
 * @details DLLからのエクスポート定義と動的関数呼び出しテンプレートクラス
 * @author yy981
 * @version 1.0
 * @note Windows専用
 */
#ifdef BUILD
    #define DLL extern "C" __declspec(dllexport) ///< DLLエクスポート定義（BUILD時）
#else
    /**
     * @brief DLLから動的に関数を呼び出すテンプレートクラス
     * @tparam R 関数の戻り値型
     * @tparam Args 関数の引数型（可変長）
     * 
     * @details
     * DLLファイルから指定された関数ポインタを取得し、
     * operator()で関数のように呼び出すことが可能
     */
    #pragma once
	#include <windows.h>
	#include <string>
	#include <cstdint>
	#include <stdexcept>
	// 関数テンプレートオブジェクト
	template<typename R, typename... Args>
	class DLL {
	public:
		/**
		 * @brief コンストラクタ
		 * @param hModule DLLモジュールハンドル
		 * @param functionName DLL内の関数名
		 */
		DLL(HMODULE hModule, std::string functionName): func(nullptr), hModule(hModule) {functionName = functionName.c_str();}

		/**
		 * @brief 関数呼び出し演算子
		 * @tparam Args 関数の引数型
		 * @param args 関数に渡す引数
		 * @return 関数の戻り値
		 * @exception std::runtime_error 関数が見つからない場合
		 * @details 初回呼び出し時に関数ポインタを取得し、キャッシュします
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
		R(*func)(Args...); ///< DLL内の関数ポインタ
		HMODULE hModule;   ///< DLLモジュールハンドル
		const char* functionName; ///< 関数名
	};
	
	/**
	 * @brief DLLモジュールを解放
	 * @param dll DLLハンドルへの参照
	 * @details ハンドルをNULLPTRで初期化
	 */
	static void cleanDLL(HMODULE& dll) {
		if (dll) {
			FreeLibrary(dll);
			dll = nullptr;
		}
	}
#endif