cmake_minimum_required(VERSION 4.2.1)

project(
        Sandbox
        VERSION 0.1.0
        LANGUAGES C CXX
)


#-------------------------------------------------
# CMAKE Language Config
#-------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HDRS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${HDRS_DIR}/*.h")

#-------------------------------------------------
# Project Executable
#-------------------------------------------------
add_executable(Sandbox ${SOURCES})
target_include_directories(Sandbox PUBLIC "${HDRS_DIR}")
target_link_libraries(Sandbox PUBLIC Lightyear)


#-------------------------------------------------
# Utility definations
#-------------------------------------------------
function(disable_clang_tidy TARGET)
    if (TARGET ${TARGET})
        set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "")
    endif ()
endfunction()


#-------------------------------------------------
# Compile definations
#-------------------------------------------------
target_compile_definitions(Sandbox PRIVATE LY_PLATFORM_WINDOWS LY_BUILD_STATIC LY_DEBUG LY_OPENGL_DEBUG)
target_compile_options(Sandbox PRIVATE

        # ---- MSVC (native or clang-cl using MSVC frontend) ----
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>>:
        /EHsc
        /utf-8
        /permissive-
        >
        $<$<CXX_COMPILER_ID:MSVC>:
        /EHsc
        /utf-8
        /permissive-
        >

        # ---- Clang GNU mode (clang++ using MinGW) ----
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>>:
        -fexceptions
        -std=c++20
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pre-c++14-compat
        -Wno-deprecated-declarations
        -w
        >

        # ---- GCC ----
        $<$<CXX_COMPILER_ID:GNU>:
        -fexceptions
        -std=c++20
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pre-c++14-compat
        -Wno-deprecated-declarations
        -w
        -fno-dwarf2-cfi-asm
        >
)



#-------------------------------------------------
# Add definations
#-------------------------------------------------
add_definitions(-DASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
add_definitions(-DSAVED_DIR="${CMAKE_CURRENT_SOURCE_DIR}/saved")


#-------------------------------------------------
# Disable Clang-Tidy
#-------------------------------------------------
disable_clang_tidy(Lightyear)
