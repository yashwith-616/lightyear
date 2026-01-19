#pragma once

// Platform specific DLL code
#if defined(_WIN32)
#define LY_DLL_EXPORT __declspec(dllexport)
#define LY_DLL_IMPORT __declspec(dllimport)
#else
#define LY_DLL_EXPORT __attribute__((visibility("default")))
#define LY_DLL_IMPORT
#endif
