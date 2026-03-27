add_library(project_compile_options INTERFACE)

target_compile_options(project_compile_options INTERFACE

    # ---- MSVC (native or clang-cl) ----
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>>:
        /EHsc
        /utf-8
        /permissive-
    >

    # ---- MSVC (Native compiler)
    $<$<CXX_COMPILER_ID:MSVC>:
        /EHsc
        /utf-8
        /permissive-
        /Zc:__cplusplus
    >

    # ---- Clang GNU mode (clang++ / MinGW) ----
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>>:
        -fexceptions
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pre-c++14-compat
        -Wno-deprecated-declarations
        -w
    >

    # ---- GCC ----
    $<$<CXX_COMPILER_ID:GNU>:
        -fexceptions
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pre-c++14-compat
        -Wno-deprecated-declarations
        -w
        -Wa,-mbig-obj
    >

    # Enable this option if issues occur in gcc debug build due to increase in size of debug frame
    # $<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:Debug>>:-gdwarf-4>

)
