#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>


class loadingTitle {
public:
	loadingTitle(int size, int duration) {
		std::thread([size,duration,this]{
			char windowTitle[256];
			GetConsoleTitle(windowTitle,256);
			std::wstring loadTitle(size, L'□'); // サイズ分だけ初期化
			int count = 0;

			while (true) {
				count %= size;

				loadTitle[(count == 0 ? size - 1 : count - 1)] = L'□';
				loadTitle[count] = L'■';

				SetConsoleTitleW(loadTitle.c_str());
				std::this_thread::sleep_for(std::chrono::milliseconds(duration));

				++count;
				if (stopSignal) break;
			}
			delete this;
		}).detach();
	}
	void stop() {stopSignal=true;}
		
private:
	bool stopSignal = false;
};