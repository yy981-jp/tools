/**
 * @file random.h
 * @brief 乱数生成機能を提供
 * @details 整数の乱数生成とシードを組み合わせたランダム文字列生成
 * @author yy981
 * @version 1.0
 */
#pragma once
#include <random>
#include <chrono>
#include <sstream>


/**
 * @brief 指定範囲内のランダムな整数を生成
 * @param low 最小値（含まれる）
 * @param up 最大値（含まれる）
 * @return lowからupの間のランダムな整数
 * @details std::mt19937とstd::random_deviceを使用
 */
inline int randomNum(const int& low, const int& up) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(low, up);
	return distribution(gen);
}

/**
 * @brief ランダムシードを生成
 * @return エンジン出力と現在の高分解能時刻を結合したランダムシード文字列
 * @details
 * std::mt19937_64と高分解能クロックを組み合わせて一意なシード文字列を生成
 * (デバッグやセッションIDなど一意性が必要な場合に有用)
 */
inline std::string randomSeed() {
	std::random_device rd;
	std::mt19937_64 eng(rd());
	auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::stringstream ss;
	ss << eng() << now;
	return ss.str();
}