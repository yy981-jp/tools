#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>


std::string getCurrentTime(const std::string format) {
	const std::time_t t = std::time(nullptr);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&t), format.c_str());
	return oss.str();
}