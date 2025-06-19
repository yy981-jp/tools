#pragma once
#include <thread>
#include <tuple>
#include <utility>
#include <yy981/time.h>


class dthread {
public:
	template <typename Func, typename... Args>
	dthread(tu unit, double value, Func&& func, Args&&... args) {
		if (running) stop();
		running=true;
		std::thread([this, unit, value, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
			while (running) {
				std::apply(func, args);
				sleepc(unit,value);
			}
		}).detach();
	}

	template <typename Func, typename... Args>
	dthread(bool noSleep, Func&& func, Args&&... args) {
		if (running) stop();
		running=true;
		std::thread([this, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
			while (running) {
				std::apply(func, args);
			}
		}).detach();
	}

	~dthread() {running=false;}
	inline void stop() {running=false;}
	
	operator bool() {
		return running;
	}

private:
	std::atomic<bool> running{false};
};