vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO gabime/spdlog
        REF v1.14.1
        SHA256 auto
)

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set(SPDLOG_FWRITE_UNLOCKED OFF CACHE BOOL "" FORCE)
endif()


vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup()