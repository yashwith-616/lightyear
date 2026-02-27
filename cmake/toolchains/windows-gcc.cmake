set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Ensure static GCC libstdc++ is allowed if desired
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "")

# Prevent MSVC fallback
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")