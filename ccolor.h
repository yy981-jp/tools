#pragma once
#include <windows.h>


namespace cc {
	inline void set(DWORD flag) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), flag);
	}

	inline void reset() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		// SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	}
}