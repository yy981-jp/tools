/**
 * @file time.h
 * @brief 時間管理とスリープ機能を提供
 * @details 複数の時間単位をサポートしたスリープ、現在時刻取得など
 * @author yy981
 * @version 1.0
 */
#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <array>
#include <chrono>
#include <thread>


/**
 * @enum tu
 * @brief 時間単位の列挙型
 */
// time unit
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
 * @brief 指定された時間、スレッドをスリープ
 * @param unit 時間単位（tu型）
 * @param value 待機時間
 * @exception std::invalid_argument 無効な時間単位が指定された場合
 * @details
 * 日・月・年単位はサポートされていない（tu::d, tu::o, tu::yで例外）
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
 * @brief 現在時刻をフォーマット文字列で取得
 * @param format strftime形式のフォーマット文字列
 * @return フォーマットされた時刻文字列
 * @note 例: getCTime("%Y-%m-%d %H:%M:%S")
 */
inline std::string getCTime(const std::string format) {
	const std::time_t t = std::time(nullptr);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&t), format.c_str());
	return oss.str();
}

/**
 * @brief Unix時刻（エポックからの秒数）を取得
 * @return 現在のUnix時刻
 */
inline int64_t getUnixTime() {
	return static_cast<int64_t>(std::time(nullptr));
}


/**
 * @brief 現在時刻の指定単位の値を取得
 * @param unit 取得する時間単位（tu型）
 * @return 秒数・分数・時数・日・月・年の値
 * @exception std::invalid_argument 無効な時間単位が指定された場合
 * @details
 * - tu::s: 秒 (0-59)
 * - tu::m: 分 (0-59)
 * - tu::h: 時 (0-23)
 * - tu::d: 日 (1-31)
 * - tu::o: 月 (1-12)
 * - tu::y: 年
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
 * @brief 総秒数を時間・分・秒に分割
 * @param total_seconds 総秒数
 * @return {時間, 分, 秒} の配列
 * @details 例: 3661秒 -> {1, 1, 1} (1時間1分1秒)
 */
inline std::array<int,3> splitTime(int total_seconds) {
	int hours = total_seconds / 3600;
	int minutes = (total_seconds % 3600) / 60;
	int seconds = total_seconds % 60;
	return {hours, minutes, seconds};
}