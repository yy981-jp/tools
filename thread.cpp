#include "thread.h"


	template <typename Func, typename... Args>
	dthread::dthread(tu unit, double value, Func&& func, Args&&... args) {
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
	dthread::dthread(bool noSleep, Func&& func, Args&&... args) {
		if (running) stop();
		running=true;
		std::thread([this, func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
			while (running) {
				std::apply(func, args);
			}
		}).detach();
	}

	dthread::~dthread() {running=false;}
	
	dthread::operator bool() {
		return running;
	}

	std::atomic<bool> running{false};
