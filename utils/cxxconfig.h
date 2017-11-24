#pragma once

// Select platform  ......
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)	//Microsoft Windows
	#define BPM_WINDOWS

	// MINGW ...
	#if defined(__MINGW32__) || defined(__MINGW64__) || defined(MINGW)
		#define BPM_MINGW
	#endif // MINGW

#elif defined(APPLE)	//Mac OS X
	//Symbols for MACOS

	#define BPM_MACOS
	#define BPM_X11

#elif (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)	//Linux
	#define BPM_LINUX
	#define BPM_X11
#else
	static_assert(false, "Only Windows and Unix are supported now (Mac OS is experimental)");
#endif

//Define a symbol for POSIX operating system.
#if defined(BPM_LINUX) || defined(BPM_MACOS)
	#define BPM_POSIX
#endif
