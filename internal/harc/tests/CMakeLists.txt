cmake_minimum_required(VERSION 3.20)

# Get sources from main Harc executable target
get_target_property(HARC_SOURCES Harc SOURCES)
list(REMOVE_ITEM HARC_SOURCES "src/Main.cpp")
list(TRANSFORM HARC_SOURCES PREPEND "../")

# Get libraries used by main Harc executable target
get_target_property(HARC_LIBRARIES Harc LINK_LIBRARIES)

# Get compile definitions use by main Harc executable
get_target_property(HARC_COMPILE_DEFINITIONS Harc COMPILE_DEFINITIONS)

add_executable(Harc_tests
    ./Harc_tests.cpp

    ${HARC_SOURCES}
)

target_compile_definitions(Harc_tests PUBLIC ${HARC_COMPILE_DEFINITIONS})
target_include_directories(Harc_tests PUBLIC ../include/)
target_link_libraries(Harc_tests PUBLIC gtest ${HARC_LIBRARIES})
