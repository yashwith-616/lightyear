#pragma once

// API Export
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

#ifdef LY_ENABLE_DEPRECATION_WARNINGS
    #ifdef _WIN32
        #if defined(LY_BUILD_STATIC)
            #define LY_DEPRECATED(msg)
        #else
            #define LY_DEPRECATED(msg) __declspec(deprecated(msg))
        #endif
    #else
        #if defined(LY_BUILD_STATIC)
            #define LY_DEPRECATED(msg)
        #else
            #define LY_DEPRECATED(msg) __attribute__((deprecated(msg)))
        #endif
    #endif
#else
    #define LY_DEPRECATED(msg)
#endif