#pragma once

#ifdef _WIN32
    #if defined(LY_BUILD_STATIC)
        #define LIGHTYEAR_API
    #elif defined(LY_BUILD_DLL)
        #define LIGHTYEAR_API __declspec(dllexport)
    #else
        #define LIGHTYEAR_API __declspec(dllimport)
    #endif
#else
    #if defined(LY_BUILD_STATIC)
        #define LIGHTYEAR_API
    #else
        #define LIGHTYEAR_API __attribute__((visibility("default")))
    #endif
#endif

#include "Core/Log.h"
#include "Core/Assertions.h"
#include "Core/Base.h"