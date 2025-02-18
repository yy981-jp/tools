#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

void return_u(const std::string i, bool noText = false) {
	if (!noText) std::cout << "Usage: " << i << std::endl;
	else std::cout << i << std::endl;
	exit(0);
}

void return_e(const std::string i, int returnCode = 1) {
	std::cerr << "ERROR: " << i << std::endl;
	exit(returnCode);
}
