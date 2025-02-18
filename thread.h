#pragma once
#include <tuple>
#include <utility>
#include <yy981/sleep.h>


class dthread {
public:
	template <typename Func, typename... Args>
	dthread(ts unit, double value, Func&& func, Args&&... args) {
		if (running) stop();
		running=true;
		std::thread([this, unit, value, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
			while (running) {
				std::apply(func, args);
				sleepc(unit,value);
			}
		}).detach();
	}
/* 修正保留
	template <typename Func, typename... Args>
	dthread(Func&& func, Args&&... args) {
		if (running) stop();
		running=true;
		std::thread([this, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
			while (running) {
				std::apply(func, args);
			}
		}).detach();
	}
*/

	~dthread() {running=false;}
	void stop() {running=false;}
	
	operator bool() {
		return running;
	}

private:
	std::atomic<bool> running{false};
};