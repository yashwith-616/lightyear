cmake_minimum_required(VERSION 3.20)

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
add_compile_options(/EHsc -Wall)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HDRS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${HDRS_DIR}/*.h")

#-------------------------------------------------
# Project Executable
#-------------------------------------------------
add_executable(Sandbox ${SOURCES})
target_include_directories(Sandbox PUBLIC "${HDRS_DIR}")
target_link_libraries(Sandbox PRIVATE Lightyear)


#-------------------------------------------------
# Compile definations
#-------------------------------------------------
target_compile_definitions(Sandbox PRIVATE LY_PLATFORM_WINDOWS LY_BUILD_STATIC LY_DEBUG)


#-------------------------------------------------
# Add definations
#-------------------------------------------------
add_definitions(-DASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")