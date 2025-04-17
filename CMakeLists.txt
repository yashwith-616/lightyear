cmake_minimum_required(VERSION 3.20)
project(LightYearWorkspace)


##################################################
# Add the subprojects
##################################################
add_subdirectory(LightYear)
add_subdirectory(Sandbox)


##################################################
# Set build output directories
##################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


##################################################
# Structure them per configuration (Debug/Release)
##################################################
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach()


##################################################
# Clang Tidy Setup
##################################################
find_program(CLANG_TIDY "clang-tidy")
if (CLANG_TIDY)
	message(STATUS "Found clang tidy: ${CLANG_TIDY}")
	set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}")
else()
	message(STATUS "clang-tidy not found, skipping clang-tidy integration")
endif()


##################################################
# Clang Format Setup
##################################################
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
	message(STATUS "Found clang-format: ${CLANG_FORMAT}")
    set(CLANGFORMAT_EXECUTABLE ${CLANG_FORMAT})
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
    include(ClangFormat)
    target_clangformat_setup(LightYear)
    target_clangformat_setup(Sandbox)
else()
	message(STATUS "clang-format not found, skipping clang-format target creation")
endif()


##################################################
# Clang format the project on build
##################################################

