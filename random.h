#pragma once
#include <random>
#include <chrono>
#include <sstream>

/**
 * @file random.h
 * @brief 乱数生成ユーティリティ
 *
 * メルセンヌツイスタ (MT19937) を使用したランダム整数生成と、
 * 時刻ベースのユニークなシードID生成を提供します。
 */

/**
 * @brief 指定範囲内のランダムな整数を返す
 * @param low 生成する乱数の最小値（含む）
 * @param up  生成する乱数の最大値（含む）
 * @return [low, up] の範囲内のランダムな整数
 */
inline int randomNum(const int& low, const int& up) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(low, up);
	return distribution(gen);
}

/**
 * @brief 時刻と乱数を組み合わせたユニークなシード文字列を生成する
 * @return MT19937-64 の乱数値と高精度タイムスタンプを連結した文字列
 * @note セッションをまたいで衝突しにくいユニークIDが必要な場面に利用できます
 */
inline std::string randomSeed() {
	std::random_device rd;
	std::mt19937_64 eng(rd());
	auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::stringstream ss;
	ss << eng() << now;
	return ss.str();
}
