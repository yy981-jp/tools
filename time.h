#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <array>
#include <chrono>
#include <thread>


// time unit
enum class tu {
	n, // ナノ秒
	c, // マイクロ秒
	l, // ミリ秒
	s, // 秒
	m, // 分
	h, // 時間
	d, // 日
	o, // 月
	y  // 年
};

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

inline std::string getCTime(const std::string format) {
	const std::time_t t = std::time(nullptr);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&t), format.c_str());
	return oss.str();
}

inline int64_t getUnixTime() {
	return static_cast<int64_t>(std::time(nullptr));
}


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

inline std::array<int,3> splitTime(int total_seconds) {
	int hours = total_seconds / 3600;
	int minutes = (total_seconds % 3600) / 60;
	int seconds = total_seconds % 60;
	return {hours, minutes, seconds};
}