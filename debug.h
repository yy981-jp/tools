#pragma once
#include <iostream>
#include <thread>
#include <chrono>


int SL = 0;
void getSL(int custom = -100000) {
	if (custom != -100000) {std::cout << "getSL()::" << std::this_thread::get_id() << "::CustomPoint: " << custom << "\n";return;}
	SL++;
	std::cout << "SL::" << std::this_thread::get_id() << "::" << SL << "\n";
}

class checkTime {
public:
    checkTime() {
		start = std::chrono::high_resolution_clock::now();
	}

    void end() {
		std::chrono::time_point<std::chrono::high_resolution_clock> e = std::chrono::high_resolution_clock::now();
		std::chrono::duration<long long, std::milli> duration = std::chrono::duration_cast<std::chrono::milliseconds>(e - start);
		std::cout << "CheckTime: " << duration.count() << "\n";
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
/*
struct getInfo {
	getInfo(int flags) {
	enum LocationFlags {
		FileName = 0b0001,
		FunctionName = 0b0010,
		Line = 0b0100,
		Column = 0b1000
	};
}*/