cmake_minimum_required(VERSION 3.20)

#==========================================================
# Compilation options
#==========================================================

#==========================================================
# Libharc_core library
#==========================================================

add_library(libharc_core STATIC
    include/harc/Algorithms.hpp
    src/Algorithms.cpp

    include/harc/OS_utils.hpp
    src/OS_utils.cpp

    include/harc/Strings.hpp
    src/Strings.cpp

    include/harc/ir/IR.hpp
    src/ir/IR.cpp

    include/harc/ir/Operations.hpp
    src/ir/Operations.cpp

    include/harc/Caching.hpp
    src/Caching.cpp

    include/harc/Message_buffer.hpp
    src/Message_buffer.cpp

    include/harc/unicode/Unicode_tables.hpp
    include/harc/Util.hpp
)

target_include_directories(libharc_core PUBLIC ./include)
target_compile_features(libharc_core PUBLIC cxx_std_20)
target_link_libraries(libharc_core PUBLIC AUL fmt)

if(HARC_USE_POSIX)
    target_compile_definitions(libharc_core PRIVATE HARC_POSIX=1)
endif()

#==========================================================
# Libharc_core test subdirectories
#==========================================================

add_subdirectory(tests/unicode/)
add_subdirectory(tests/)
