/**
 * @file def.h
 * @brief マクロと定義をまとめたヘッダ
 * @details コマンドライン引数の簡易マクロと、グローバル変数の定義
 * @author yy981
 * @version 1.0
 */
#define ARGC int argc, char *argv[] ///< コマンドライン引数の定義マクロ
#if defined(DEF_FS)
	#pragma once
	#include <filesystem>
	namespace fs = std::filesystem; ///< std::filesystem のエイリアス
#endif
int Nargc = 1; ///< デフォルトのargc値
constexpr char **Nargv = {}; ///< デフォルトのargv値
const