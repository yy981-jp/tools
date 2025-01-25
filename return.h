#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

void return_u(std::string i) {
	std::cout << "Usage: " + i << std::endl;
	exit(0);
}

void return_e(std::string i, int returnCode = 1) {
	std::cerr << "ERROR: " << i << std::endl;
	exit(returnCode);
}
