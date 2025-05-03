#pragma once

#include "Log.h"

#ifdef _MSC_VER  // Microsoft compiler
    #define LY_DEBUGBREAK() __debugbreak()
#else
    #define LY_DEBUGBREAK() std::abort()
#endif

#ifdef LY_DEBUG
    #define LY_ENABLE_ASSERTS
#endif

#ifdef LY_ENABLE_ASSERTS
    #define LY_ASSERT(x, ...)                                                                      \
        {                                                                                          \
            if (!(x)) {                                                                            \
                LY_LOG(ly::LogType::Error, "Assertion failed: {0}", __VA_ARGS__);                  \
                LY_DEBUGBREAK()                                                                    \
            }                                                                                      \
        }
    #define LY_CORE_ASSERT(x, ...)                                                                 \
        {                                                                                          \
            if (!(x)) {                                                                            \
                LY_CORE_LOG(ly::LogType::Error, "Assertion failed: {0}", __VA_ARGS__);             \
                LY_DEBUGBREAK();                                                                   \
            }                                                                                      \
        }
#else
    #define LY_ASSERT(x, ...)
    #define LY_CORE_ASSERT(x, ...)
#endif
