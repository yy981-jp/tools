#pragma once
#include <random>


inline int randomNum(const int& low, const int& up) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(low, up);
	return distribution(gen);
}