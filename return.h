/**
 * @file return.h
 * @brief 終了コードを指定してプログラムを終了するユーティリティ関数
 * @details 使用方法やエラーメッセージを出力して終了
 * @author yy981
 * @version 1.0
 */
#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

/**
 * @brief 使用方法を出力してプログラムを終了
 * @param i 出力メッセージ
 * @param noText true: メッセージをそのまま出力、false: "Usage: " プレフィックス付きで出力
 * @note 終了コード: 0
 */
inline void return_u(const std::string i, bool noText = false) {
	if (!noText) std::cout << "Usage: " << i << std::endl;
	else std::cout << i << std::endl;
	exit(0);
}

/**
 * @brief エラーメッセージを出力してプログラムを終了
 * @param i エラーメッセージ
 * @param returnCode 終了コード（デフォルト: 1）
 * @note "ERROR: " プレフィックス付きで標準エラー出力に出力
 */
inline void return_e(const std::string i, int returnCode = 1) {
	std::cerr << "ERROR: " << i << std::endl;
	exit(returnCode);
}
