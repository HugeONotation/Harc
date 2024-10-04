cmake_minimum_required(VERSION 3.20)

#==========================================================
# Compilation options
#==========================================================

option(HARC_FRONTEND_USE_POSIX ON)

option(HARC_FRONTEND_USE_CUDA OFF)

#==========================================================
# External libraries
#==========================================================

find_package(ICU 70.1 COMPONENTS uc i18n REQUIRED)

#==========================================================
# Libharc_frontend library
#==========================================================

add_library(libharc_frontend STATIC
    include/harc/lexer/Lexer.hpp
    src/lexer/Lexer.cpp

    include/harc/lexer/Lexing_errors.hpp

    include/harc/lexer/Tokens.hpp
    src/lexer/Tokens.cpp

    ../libharc_core/include/harc/unicode/Unicode_tables.hpp

    include/harc/lexer/Cache.hpp
    src/lexer/Cache.cpp
)

target_link_libraries(libharc_frontend PUBLIC libharc_core ICU::uc ICU::i18n AUL)

target_include_directories(libharc_frontend PUBLIC ./include/)

#==========================================================
# Tests
#==========================================================

add_subdirectory(tests)
