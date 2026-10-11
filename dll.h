#pragma once

#ifdef BUILD
	#define DLL __declspec(dllexport)
#else
	#include <windows.h>

	#include <stdexcept>
	#include <string>
	#include <utility>

	/**
	 * @file dll.h
	 * @brief DLL 動的ロードユーティリティ
	 *
	 * BUILD が定義されている場合:
	 *	DLL は __declspec(dllexport) に展開されます。
	 *
	 * BUILD が定義されていない場合:
	 *	DllModule が DLL のロード・解放を管理し、
	 *	DLL<R, Args...> が関数の遅延解決・呼び出しを行います。
	 */

	/**
	 * @class DllModule
	 * @brief DLL モジュールの所有権を管理するクラス
	 *
	 * LoadLibraryW() で DLL をロードし、
	 * デストラクタで FreeLibrary() を呼び出します。
	 *
	 * コピーは禁止し、ムーブのみ許可します。
	 */
	class DllModule {
	public:
		/**
		 * @brief DLL をロードする
		 * @param path DLL ファイルのパス
		 * @throws std::runtime_error ロードに失敗した場合
		 */
		explicit DllModule(const wchar_t* path): handle_(LoadLibraryW(path)) {
			if (!handle_) {
				const DWORD error = GetLastError();

				throw std::runtime_error(
					"LoadLibraryW failed (error " +
					std::to_string(error) + ")"
				);
			}
		}

		~DllModule() {
			reset();
		}

		DllModule(const DllModule&) = delete;
		DllModule& operator=(const DllModule&) = delete;

		DllModule(DllModule&& other) noexcept:
			handle_(std::exchange(other.handle_, nullptr)) {}

		DllModule& operator=(DllModule&& other) noexcept {
			if (this != &other) {
				reset();
				handle_ = std::exchange(other.handle_, nullptr);
			}

			return *this;
		}

		/**
		 * @brief DLL モジュールハンドルを取得する
		 */
		HMODULE get() const noexcept {
			return handle_;
		}

		/**
		 * @brief DLL を解放する
		 */
		void reset() noexcept {
			if (handle_) {
				FreeLibrary(handle_);
				handle_ = nullptr;
			}
		}

		explicit operator bool() const noexcept {
			return handle_ != nullptr;
		}

	private:
		HMODULE handle_{};
	};

	/**
	 * @class DLL
	 * @brief DLL の関数を遅延解決して呼び出すファンクター
	 *
	 * 初回呼び出し時に GetProcAddress() で関数を解決し、
	 * 以降はキャッシュした関数ポインタを使用します。
	 *
	 * このクラスは DLL モジュールの所有権を持ちません。
	 * 使用する DllModule が生存している間に呼び出してください。
	 *
	 * @tparam R 戻り値型
	 * @tparam Args 引数型
	 */
	template<typename R, typename... Args>
	class DLL {
	public:
		/**
		 * @brief 関数ラッパーを作成する
		 * @param hModule DLL モジュールハンドル
		 * @param functionName 関数名
		 */
		DLL(HMODULE hModule, std::string functionName):
			hModule_(hModule),
			functionName_(std::move(functionName)) {
			if (!hModule_) {
				throw std::invalid_argument(
					"DLL module handle is null"
				);
			}
		}

		/**
		 * @brief 関数を呼び出す
		 * @throws std::runtime_error 関数の解決に失敗した場合
		 */
		R operator()(Args... args) {
			if (!func_) {
				func_ = reinterpret_cast<Function>(
					GetProcAddress(
						hModule_,
						functionName_.c_str()
					)
				);

				if (!func_) {
					const DWORD error = GetLastError();

					throw std::runtime_error(
						"GetProcAddress failed for '" +
						functionName_ +
						"' (error " +
						std::to_string(error) + ")"
					);
				}
			}

			return func_(std::forward<Args>(args)...);
		}

	private:
		using Function = R(*)(Args...);

		HMODULE hModule_;
		std::string functionName_;
		Function func_ = nullptr;
	};
#endif
