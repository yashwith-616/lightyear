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
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${HDRS_DIR}/*.h")


#-------------------------------------------------
# Optional Parameters
#-------------------------------------------------
option(LY_DEBUG_ENABLED "Enable debug such as assertions and logs" ON)
option(LY_GRAPHICS_DEBUG "Enable graphic context debugging" ON)
option(LY_ENABLE_DEPRECATION_WARNINGS "Enable deprecations warning" ON)


#-------------------------------------------------
# Utility definations
#-------------------------------------------------
function(disable_clang_tidy TARGET)
    if(TARGET ${TARGET})
        set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "")
    endif()
endfunction()


#-------------------------------------------------
# Setup Dependencies
#-------------------------------------------------
find_package(OpenGL REQUIRED )
include_directories(${OPENGL_INCLUDE_DIRS})

# Glad
set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/glad")
add_library(glad STATIC "${GLAD_DIR}/glad.c")
target_include_directories(glad PRIVATE "${GLAD_DIR}")

# GLFW
set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the glfw example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the glfw test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the glfw documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLFW_DIR} "glfw-binaries")

# ImGUI
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui")
add_library(imgui STATIC
  "${IMGUI_DIR}/imgui.cpp"
  "${IMGUI_DIR}/imgui_demo.cpp"
  "${IMGUI_DIR}/imgui_draw.cpp"
  "${IMGUI_DIR}/imgui_widgets.cpp"
  "${IMGUI_DIR}/imgui_tables.cpp"
  "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
  "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PRIVATE "${IMGUI_DIR}")
target_include_directories(imgui PRIVATE "${GLFW_DIR}/include")
target_link_libraries(imgui PUBLIC glfw ${OPENGL_LIBRARIES})

# GLM
set(GLM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/glm")

# STB
set(STB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/stb")

# SPDLOG
set(SPDLOG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/spdlog")

# ENTT
set(ENTT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/entt/single_include")

# REFL-CPP
set(REFL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/refl-cpp")

#-------------------------------------------------
# Project Library
#-------------------------------------------------
add_library (Lightyear STATIC ${SOURCES} "${HEADERS}")
target_include_directories(Lightyear
    PUBLIC
        "${HDRS_DIR}"
    SYSTEM PUBLIC
        "${IMGUI_DIR}"
        "${GLAD_DIR}"
        "${GLM_DIR}"
        "${ENTT_DIR}"
        "${SPDLOG_DIR}/include"
        "${REFL_DIR}/include"
    SYSTEM PRIVATE
        "${GLFW_DIR}/include"
        "${STB_DIR}"
)

target_link_libraries(Lightyear
    PUBLIC 
        imgui
        $<$<BOOL:${MINGW}>:ws_32>
        glad
    PRIVATE
        glfw
)

#-------------------------------------------------
# Compile definations
#-------------------------------------------------
target_compile_definitions(Lightyear 
	PRIVATE 
	#LY_BUILD_DLL
    LY_BUILD_STATIC
	LY_PLATFORM_WINDOWS 
	GLFW_INCLUDE_NONE
	STB_IMAGE_IMPLEMENTATION
    SPDLOG_HEADER_ONLY 
    FMT_HEADER_ONLY
)

target_compile_options(Lightyear PRIVATE
    # MSVC
    $<$<CXX_COMPILER_ID:MSVC>:/EHsc>

    # Clang on Windows (clang-cl)
    $<$<AND:$<CXX_COMPILER_ID:Clang>>:
        -fexceptions
        -std=c++20
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pre-c++14-compat
        -Wno-deprecated-declarations
        -w  # disables all warnings (optional extreme)
    >
)
target_compile_options(Lightyear PRIVATE /EHsc)

#-------------------------------------------------
# Optional Compile definations
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
