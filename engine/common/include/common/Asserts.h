#pragma once

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
    #include <intrin.h>
    #define LY_DEBUGBREAK() __debugbreak()
#else
    #include <signal.h>
    #define LY_DEBUGBREAK() raise(SIGTRAP)
#endif

namespace ly::detail {

inline void assertFail(char const* expr, char const* msg, char const* file, int line) {
    std::fprintf(stderr,
                 "Assertion failed!\n"
                 "  Expr: %s\n"
                 "  Msg : %s\n"
                 "  File: %s:%d\n",
                 expr,
                 msg ? msg : "<none>",
                 file,
                 line);

    std::fflush(stderr);
    LY_DEBUGBREAK();
    std::abort();
}

}  // namespace ly::detail

// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#ifdef LY_ENABLE_ASSERTS

    #define LY_ASSERT(x, ...)                                                                                          \
        do {                                                                                                           \
            if (!(x)) {                                                                                                \
                ly::detail::assertFail(#x, "" __VA_ARGS__, __FILE__, __LINE__);                                        \
            }                                                                                                          \
        } while (false)

#else

    #define LY_ASSERT(x, ...)

#endif

// NOLINTEND(cppcoreguidelines-avoid-do-while)
