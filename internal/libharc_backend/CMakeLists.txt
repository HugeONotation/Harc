cmake_minimum_required(VERSION 3.20)

#==========================================================
# Compilation options
#==========================================================

option(HARC_BACKEND_USE_POSIX ON)

option(HARC_BACKEND_USE_CUDA OFF)

#==========================================================
# Libharc_backend library
#==========================================================

add_library(libharc_backend STATIC
    include/harc/arch/x86_64/Instruction.hpp
    src/arch/x86_64/Instruction.cpp
)

target_compile_features(libharc_backend PUBLIC cxx_std_20)

target_compile_options(libharc_backend PUBLIC "$<$<CONFIG:DEBUG>:-fsanitize=address>")
target_link_options(libharc_backend PUBLIC "$<$<CONFIG:DEBUG>:-fsanitize=address>")

target_link_libraries(libharc_backend PUBLIC libharc_core PRIVATE AUL xed)

target_include_directories(libharc_backend PUBLIC ./include/)
