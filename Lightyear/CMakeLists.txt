cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


#-------------------------------------------------
# Project
#-------------------------------------------------
project (
	Lightyear
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
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS  "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS  "${HDRS_DIR}/*.h")


#-------------------------------------------------
# Optional Parameters
#-------------------------------------------------
option(LY_DEBUG_ENABLED "Enable debug such as assertions and logs" ON)
option(LY_GRAPHICS_DEBUG "Enable graphic context debugging" ON)
option(LY_ENABLE_DEPRECATION_WARNINGS "Enable deprecations warning" ON)


#-------------------------------------------------
# Utility definitions
#-------------------------------------------------
function(disable_clang_tidy TARGET)
    if(TARGET ${TARGET})
        set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "")
    endif()
endfunction()


#-------------------------------------------------
# Configuration Files
#-------------------------------------------------
set(FILE_HEADER_MAGIC "LYFS")
set(FILE_VERSION 1)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/Config.hpp.in
        ${CMAKE_CURRENT_SOURCE_DIR}/config/Config.hpp
        @ONLY
)

#-------------------------------------------------
# Setup Dependencies
#-------------------------------------------------
find_package(OpenGL REQUIRED )
include_directories(${OPENGL_INCLUDE_DIRS})

#cereal
find_package(cereal CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unordered_dense CONFIG REQUIRED)
find_path(REFL_CPP_INCLUDE_DIRS "refl.hpp")

# Configure packages
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)



#-------------------------------------------------
# Project Library
#-------------------------------------------------
add_library (Lightyear STATIC ${SOURCES} "${HEADERS}")
target_include_directories(Lightyear
    PUBLIC
        "${HDRS_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/config"
    SYSTEM PUBLIC
        ${REFL_CPP_INCLUDE_DIRS}
    SYSTEM PRIVATE
        ${Stb_INCLUDE_DIR}
)

target_link_libraries(Lightyear
    PUBLIC
        $<$<BOOL:${MINGW}>:ws2_32>
        cereal::cereal
        EnTT::EnTT
        glad::glad
        glfw
        glm::glm
        imgui::imgui
        spdlog::spdlog_header_only
        unordered_dense::unordered_dense
    PRIVATE
        glfw
)

#-------------------------------------------------
# Compile definitions
#-------------------------------------------------
target_compile_definitions(Lightyear 
	PRIVATE 
	#LY_BUILD_DLL
    LY_BUILD_STATIC
	LY_PLATFORM_WINDOWS 
	GLFW_INCLUDE_NONE
	STB_IMAGE_IMPLEMENTATION
)

target_compile_options(Lightyear PRIVATE
        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>:
        /EHsc
        /utf-8
        /permissive-
        >

        # Clang on Windows (clang-cl)
        $<$<AND:$<CXX_COMPILER_ID:Clang>>:
        -fexceptions
        -std=c++20
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pre-c++14-compat
        -Wno-deprecated-declarations
        -w
        >

        # GCC / MinGW
        $<$<CXX_COMPILER_ID:GNU>:
        -w
        >
)

#-------------------------------------------------
# Optional Compile definitions
#-------------------------------------------------
if (LY_GRAPHICS_DEBUG)
	target_compile_definitions(Lightyear PRIVATE LY_OPENGL_DEBUG)
endif()

if (LY_ENABLE_DEPRECATION_WARNINGS)
	target_compile_definitions(Lightyear PUBLIC LY_ENABLE_DEPRECATION_WARNINGS)
endif()

if (LY_DEBUG_ENABLED)
	target_compile_definitions(Lightyear PRIVATE LY_DEBUG)
endif()


#-------------------------------------------------
# Pre-Compiled Headers
#-------------------------------------------------
target_precompile_headers(Lightyear PRIVATE "${HDRS_DIR}/Lightyear/pch/lypch.h")


#-------------------------------------------------
# Post build step
#-------------------------------------------------
add_custom_command(TARGET Lightyear POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Lightyear>
    $<TARGET_FILE_DIR:Sandbox>
)

add_custom_command(TARGET Lightyear POST_BUILD
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clangformat
	COMMENT "Running clang-format"
)


#-------------------------------------------------
# Disable Clang-Tidy
#-------------------------------------------------
disable_clang_tidy(imgui)
disable_clang_tidy(glfw)
disable_clang_tidy(glad)
