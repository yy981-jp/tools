#pragma once
#include <string>
#include <cstdlib>


std::string normalize_path(std::string str) {
	size_t pos = 0;
	while ((pos = str.find('\\', pos)) != std::string::npos) {
		str.replace(pos, 1, "/");
		pos++;
	}
	return str;
}

std::string normalize_path(const char* str) {
	return normalize_path(std::string(str));
}