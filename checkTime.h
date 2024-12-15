#pragma once
#include <iostream>
#include <chrono>


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