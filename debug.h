#pragma once
#include <iostream>
#include <thread>


int SL = 0;
void getSL(int custom = -100000) {
	if (custom != -100000) {std::cout << "getSL()::" << std::this_thread::get_id() << "::CustomPoint: " << custom << "\n";return;}
	SL++;
	std::cout << "SL::" << std::this_thread::get_id() << "::" << SL << "\n";
}