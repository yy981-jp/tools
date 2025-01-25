#warning y9INC: This code has not been tested yet
#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <tuple>
#include <utility> // std::forward, std::apply


// 汎用的な非同期実行関数
template <typename Func, typename... Args>
void dthread(std::chrono::duration time, Func&& func, Args&&... args) {
	std::thread([time, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
		while (true) {
			std::apply(func, args);
			std::this_thread::sleep_for(time);
		}
	}).detach();
}