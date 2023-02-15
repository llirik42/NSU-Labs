cmake_minimum_required(VERSION 3.23)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 17)

set(exe_name ${PROJECT_NAME})
set(test_name Tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(FetchContent)
FetchContent_Declare(
        argparse
        URL https://github.com/p-ranav/argparse/archive/refs/tags/v2.9.zip
)
FetchContent_MakeAvailable(argparse)

add_library(corelib OBJECT
        src/config_parser.cpp
        src/converters_factory.cpp
        src/processor.cpp
        src/streams.cpp
        src/utils.cpp
        src/wav.cpp

        src/converters/converters_utils.cpp
        src/converters/cut_converter.cpp
        src/converters/insert_converter.cpp
        src/converters/mix_converter.cpp
        src/converters/volume_converter.cpp
        )

add_executable(${exe_name} src/main.cpp)

target_link_libraries(${exe_name} corelib argparse)

target_compile_options(corelib PUBLIC
        -Wall
        -Wextra
        -pedantic
        -Werror
        -Wctor-dtor-privacy # Class seems unusable because all the constructors or destructors in that class are private
        -Wnon-virtual-dtor # Class has virtual functions and an accessible non-virtual destructor
        -Wold-style-cast # Old-style cast to a non-void type is used
        -Woverloaded-virtual #  Function declaration hides virtual functions from a base class
        -Wsign-promo # Non-obvious choice of an overloaded function
        -Wduplicated-branches # if-else has identical branches
        -Wduplicated-cond # Duplicated conditions in an if-else-if chain
        -Wconversion # Implicit conversions that may alter a value
        -Wsign-conversion # implicit conversions that may change the sign of an integer value
        -Wzero-as-null-pointer-constant # 0 is used as null pointer constant
        -Wextra-semi # Redundant semicolon after in-class function definition
        -Wlogical-op # Suspicious uses of logical operators in expressions
        )

add_executable(${test_name} tests/wav_tests.cpp tests/config_parser_tests.cpp)
target_link_libraries(${test_name} gtest_main)
target_link_libraries(${test_name} corelib)
