cmake_minimum_required(VERSION 3.20)
project(lightyear)


#-------------------------------------------------
# Add the subprojects
#-------------------------------------------------
add_subdirectory(Lightyear)
add_subdirectory(Sandbox)
add_subdirectory(engine)

#-------------------------------------------------
# Example
#-------------------------------------------------
option(LIGHTYEAR_BUILD_EXAMPLES "Build engine examples" ON)

if (LIGHTYEAR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

#-------------------------------------------------
# Clang Tidy Setup
#-------------------------------------------------

option(LY_TIDY "Perform clang tidy analysis on lightyear on build" OFF)

if (LY_TIDY)
    find_program(CLANG_TIDY "clang-tidy")
    if (CLANG_TIDY)
        message(STATUS "Found clang tidy: ${CLANG_TIDY}")
        set(CLANG_TIDY_COMMAND
                "${CLANG_TIDY};--extra-arg=/EHsc;-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
        )
        set_target_properties(Lightyear PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
        set_target_properties(Sandbox PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    else ()
        message(STATUS "clang-tidy not found, skipping clang-tidy integration")
    endif ()
endif ()


#-------------------------------------------------
# Clang Format Setup
#-------------------------------------------------
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
        message(STATUS "Found clang-format: ${CLANG_FORMAT}")
        set(CLANGFORMAT_EXECUTABLE ${CLANG_FORMAT})
        list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
        include(ClangFormat)
        target_clangformat_setup(Lightyear)
        target_clangformat_setup(Sandbox)
else ()
        message(STATUS "clang-format not found, skipping clang-format target creation")
endif ()
