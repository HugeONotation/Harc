cmake_minimum_required(VERSION 3.20)

add_executable(libharc_core_tests
    libharc_core_tests.cpp
)

target_link_libraries(libharc_core_tests PUBLIC gtest libharc_core)
