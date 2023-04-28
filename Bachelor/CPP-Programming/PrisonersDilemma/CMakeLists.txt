cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)

project(PrisonersDilemma)

set(exe_name ${PROJECT_NAME})
set(test_name test)

add_compile_options(-Wall -Wextra -pedantic -Werror)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} src/strategies/*.cpp)
add_library(corelib OBJECT ${SOURCES}
        src/args_parser.cpp src/game.cpp src/matrix.cpp src/strategies_factory.cpp src/viewer.cpp src/storage.cpp src/random_init.cpp)

add_executable(${exe_name} src/main.cpp )
target_link_libraries(${exe_name} corelib)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${test_name} tests/matrix_test.cpp)
target_link_libraries(${test_name} gtest_main)
target_link_libraries(${test_name} corelib)
