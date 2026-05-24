#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

/**
 * @file return.h
 * @brief プログラム終了ヘルパー関数
 *
 * 使用方法メッセージやエラーメッセージを表示してプロセスを終了する
 * 簡易ユーティリティ関数を提供します。
 */

/**
 * @brief 使い方メッセージを表示してプログラムを正常終了する
 * @param i    表示する文字列（noText=false の場合 "Usage: " プレフィックスが付く）
 * @param noText true の場合はプレフィックスなしでそのまま出力する（デフォルト: false）
 * @note exit(0) を呼ぶため、この関数は返りません
 */
inline void return_u(const std::string i, bool noText = false) {
	if (!noText) std::cout << "Usage: " << i << std::endl;
	else std::cout << i << std::endl;
	exit(0);
}

/**
 * @brief エラーメッセージを stderr に出力してプログラムを異常終了する
 * @param i          表示するエラーメッセージ文字列（"ERROR: " プレフィックスが付く）
 * @param returnCode 終了コード（デフォルト: 1）
 * @note exit(returnCode) を呼ぶため、この関数は返りません
 */
inline void return_e(const std::string i, int returnCode = 1) {
	std::cerr << "ERROR: " << i << std::endl;
	exit(returnCode);
}
