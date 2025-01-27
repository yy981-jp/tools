#pragma once
#include <chrono>
#include <thread>


enum class time_s {n,c,l,s,m,h};

void sleepc(time_s unit, double value) {
	std::chrono::duration<double> converted;
	switch (unit) {
		case time_s::n: converted = std::chrono::nanoseconds(value); break;
		case time_s::c: converted = std::chrono::microseconds(value); break;
		case time_s::l: converted = std::chrono::milliseconds(value); break;
		case time_s::s: converted = std::chrono::seconds(value); break;
		case time_s::m: converted = std::chrono::minutes(value); break;
		case time_s::h: converted = std::chrono::hours(value); break;
	}
	std::this_thread::sleep_for(converted);
}
