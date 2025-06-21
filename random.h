#pragma once
#include <random>
#include <chrono>
#include <sstream>


inline int randomNum(const int& low, const int& up) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(low, up);
	return distribution(gen);
}

inline std::string randomSeed() {
	std::random_device rd;
	std::mt19937_64 eng(rd());
	auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::stringstream ss;
	ss << eng() << now;
	return ss.str();
}