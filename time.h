#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <array>


std::string getCTime(const std::string format) {
	const std::time_t t = std::time(nullptr);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&t), format.c_str());
	return oss.str();
}

std::array<int,3> splitTime(int total_seconds) {
	int hours = total_seconds / 3600;
	int minutes = (total_seconds % 3600) / 60;
	int seconds = total_seconds % 60;
	return {hours, minutes, seconds};
}