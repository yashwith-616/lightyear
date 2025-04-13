#pragma once

#ifdef _WIN32
	#ifdef LY_BUILD_DLL
		#define LIGHTYEAR_API __declspec(dllexport)
	#else
		#define LIGHTYEAR_API __declspec(dllimport)
	#endif
#else
	#define LIGHTYEAR_API __attribute__((visibility("default")))
#endif
	