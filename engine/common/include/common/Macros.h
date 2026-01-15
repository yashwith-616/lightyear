#pragma once

// ------------------------------------------
// Deprecated function use warnings
// ------------------------------------------
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

// ------------------------------------------
// ForceInline functions
// ------------------------------------------
#ifdef _MSC_VER
    #define FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define FORCEINLINE inline __attribute__((always_inline))
#else
    #define FORCEINLINE inline
#endif

// ------------------------------------------
// Push Diagnostics Macro
// ------------------------------------------
#if defined(_MSC_VER)
    #define LY_DISABLE_WARNINGS_PUSH __pragma(warning(push, 0))
    #define LY_DISABLE_WARNINGS_POP __pragma(warning(pop))
#elif defined(__clang__)
    #define LY_DISABLE_WARNINGS_PUSH                                                                                   \
        _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Weverything\"")
    #define LY_DISABLE_WARNINGS_POP _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
    #define LY_DISABLE_WARNINGS_PUSH _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wall\"")
    #define LY_DISABLE_WARNINGS_POP _Pragma("GCC diagnostic pop")
#else
    #define LY_DISABLE_WARNINGS_PUSH
    #define LY_DISABLE_WARNINGS_POP
#endif
