#define ARGC int argc, char *argv[]
#if defined(DEF_FS)
	#pragma once
	#include <filesystem>
	namespace fs = std::filesystem;
#endif
const int Nargc = 1;
constexpr char* Nargv[] = {"dummyAppString"}; const