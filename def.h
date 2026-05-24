/**
 * @file def.h
 * @brief 共通マクロ・グローバル定数定義
 *
 * argc/argv 引数の簡略表記マクロや、ファイルシステム名前空間エイリアス、
 * デフォルト引数値を定義します。
 */

/// @brief main 関数引数の省略記述マクロ。`int main(ARGC)` のように使用します
#define ARGC int argc, char *argv[]

#if defined(DEF_FS)
	#pragma once
	#include <filesystem>
	/// @brief std::filesystem の省略エイリアス（DEF_FS が定義されている場合のみ有効）
	namespace fs = std::filesystem;
#endif

/// @brief デフォルトの argc 値（引数なし状態）
int Nargc = 1;

/// @brief デフォルトの argv 値（空の引数配列）
constexpr char **Nargv = {}; const
