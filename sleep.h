#pragma once
#include <chrono>
#include <thread>


enum class time_s {n,c,l,s,m,h};

void sleepc(time_s unit, size_t value) {
	std::chrono::duration<double> converted;
	switch (unit) {
		case time_s::n: converted = std::chrono::nanoseconds(3); break;
		case time_s::c: converted = std::chrono::microseconds(3); break;
		case time_s::l: converted = std::chrono::milliseconds(3); break;
		case time_s::s: converted = std::chrono::seconds(3); break;
		case time_s::m: converted = std::chrono::minutes(3); break;
		case time_s::h: converted = std::chrono::hours(3); break;
	}
	std::this_thread::sleep_for(converted);
}