cmake_minimum_required(VERSION 3.20)

add_executable(Unicode_properties
    Unicode_properties.cpp
)

target_compile_features(Unicode_properties PRIVATE cuda_std_17)


