cmake_minimum_required(VERSION 3.20)

add_executable(Sandbox
    Sandbox.cpp
)

target_compile_features(Sandbox PUBLIC cxx_std_20)