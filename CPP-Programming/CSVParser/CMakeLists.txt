cmake_minimum_required(VERSION 3.23)
project(CSVParser)

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

add_library(corelib OBJECT src/utils.cpp)

add_executable(${exe_name} src/main.cpp)

target_link_libraries(${exe_name} corelib)

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
        -Wfloat-equal # Floating-point values are used in equality comparisons
        -Wconversion # Implicit conversions that may alter a value
        -Wsign-conversion # implicit conversions that may change the sign of an integer value
        -Wzero-as-null-pointer-constant # 0 is used as null pointer constant
        -Wextra-semi # Redundant semicolon after in-class function definition
        -Wlogical-op # Suspicious uses of logical operators in expressions
        )

add_executable(${test_name} tests/csv_parser_tests.cpp)
target_link_libraries(${test_name} gtest_main)
target_link_libraries(${test_name} corelib)
