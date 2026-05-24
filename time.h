#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <array>
#include <chrono>
#include <thread>

/**
 * @file time.h
 * @brief 時刻・時間ユーティリティ
 *
 * 時間単位の列挙型 (tu)、スリープ関数、現在時刻の取得・フォーマット、
 * Unix タイムスタンプ取得、秒数を時分秒に分割する関数を提供します。
 */

/**
 * @enum tu
 * @brief 時間単位を表す列挙型
 *
 * sleepc() や getCTime() などの時間関数に渡す単位を指定します。
 */
enum class tu {
	n, ///< ナノ秒
	c, ///< マイクロ秒
	l, ///< ミリ秒
	s, ///< 秒
	m, ///< 分
	h, ///< 時間
	d, ///< 日
	o, ///< 月
	y  ///< 年
};

/**
 * @brief 指定した時間単位でスレッドをスリープさせる
 * @param unit  時間単位 (tu::n / tu::c / tu::l / tu::s / tu::m / tu::h)
 * @param value スリープする時間の値
 * @throws std::invalid_argument サポートされていない単位 (d / o / y) が渡された場合
 */
inline void sleepc(tu unit, double value) {
	switch (unit) {
		case tu::n: std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(value)); break;
		case tu::c: std::this_thread::sleep_for(std::chrono::duration<double, std::micro>(value)); break;
		case tu::l: std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(value)); break;
		case tu::s: std::this_thread::sleep_for(std::chrono::duration<double>(value)); break;
		case tu::m: std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<60>>(value)); break;
		case tu::h: std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<3600>>(value)); break;
		default: throw std::invalid_argument("Invalid time unit");
	}
}

/**
 * @brief 現在のローカル時刻を指定フォーマットの文字列で返す
 * @param format strftime 書式文字列（例: "%Y-%m-%d %H:%M:%S"）
 * @return フォーマットされた現在時刻文字列
 */
inline std::string getCTime(const std::string format) {
	const std::time_t t = std::time(nullptr);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&t), format.c_str());
	return oss.str();
}

/**
 * @brief 現在の Unix タイムスタンプ（秒）を返す
 * @return エポック（1970-01-01 00:00:00 UTC）からの経過秒数
 */
inline int64_t getUnixTime() {
	return static_cast<int64_t>(std::time(nullptr));
}

/**
 * @brief 指定した時間単位の現在値をローカル時刻から返す
 * @param unit 取得する時間単位（tu::s / tu::m / tu::h / tu::d / tu::o / tu::y）
 * @return 対応する時刻フィールドの整数値（年は西暦、月は 1〜12、日は 1〜31）
 * @throws std::invalid_argument サポートされていない単位が渡された場合
 */
inline int getCTime(const tu unit) {
	auto current = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm current_tm = *std::localtime(&current);
	switch (unit) {
		case tu::s: return current_tm.tm_sec;
		case tu::m: return current_tm.tm_min;
		case tu::h: return current_tm.tm_hour;
		case tu::d: return current_tm.tm_mday;
		case tu::o: return current_tm.tm_mon+1;
		case tu::y: return current_tm.tm_year+1900;
		default: throw std::invalid_argument("Invalid time unit");
	}
}

/**
 * @brief 秒数を 時間・分・秒 の配列に分割する
 * @param total_seconds 分割する総秒数
 * @return {時間, 分, 秒} の順に格納した 3 要素の配列
 */
inline std::array<int,3> splitTime(int total_seconds) {
	int hours = total_seconds / 3600;
	int minutes = (total_seconds % 3600) / 60;
	int seconds = total_seconds % 60;
	return {hours, minutes, seconds};
}
