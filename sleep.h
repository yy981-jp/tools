#pragma once
#include <chrono>
#include <thread>

enum class time_s {n, c, l, s, m, h};

/**
 * @brief 指定した単位の時間だけ現在のスレッドを停止する
 * @param unit 時間単位
 * @param value 停止時間
 *
 * 浮動小数点 duration を使うことで、0.5 秒などの小数値や
 * ナノ秒・マイクロ秒単位の値も整数 duration への変換エラーなしで扱います。
 */
inline void sleepc(time_s unit, double value) {
	switch (unit) {
		case time_s::n:
			std::this_thread::sleep_for(std::chrono::duration<double, std::nano>(value));
			break;
		case time_s::c:
			std::this_thread::sleep_for(std::chrono::duration<double, std::micro>(value));
			break;
		case time_s::l:
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(value));
			break;
		case time_s::s:
			std::this_thread::sleep_for(std::chrono::duration<double>(value));
			break;
		case time_s::m:
			std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<60>>(value));
			break;
		case time_s::h:
			std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<3600>>(value));
			break;
	}
}
