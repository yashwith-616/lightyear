#pragma once

#include "Lightyear/Common/Log.h"

#ifdef _MSC_VER  // Microsoft compiler
    #define LY_DEBUGBREAK() __debugbreak()
#else
    #define LY_DEBUGBREAK() std::abort()
#endif

#ifdef LY_DEBUG
    #define LY_ENABLE_ASSERTS
#endif

// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#ifdef LY_ENABLE_ASSERTS
    #define LY_ASSERT(x, ...)                                                                                          \
        do {                                                                                                           \
            if (!(x)) {                                                                                                \
                LY_LOG(ly::LogType::Error, "Assertion failed: {0}", __VA_ARGS__);                                      \
                LY_DEBUGBREAK();                                                                                       \
            }                                                                                                          \
        } while (false)
    #define LY_CORE_ASSERT(x, ...)                                                                                     \
        do {                                                                                                           \
            if (!(x)) {                                                                                                \
                LY_LOG(ly::LogType::Error, "Assertion failed: {0}", __VA_ARGS__);                                      \
                LY_DEBUGBREAK();                                                                                       \
            }                                                                                                          \
        } while (false)
#else
    #define LY_ASSERT(x, ...)
    #define LY_CORE_ASSERT(x, ...)
#endif

// NOLINTEND(cppcoreguidelines-avoid-do-while)