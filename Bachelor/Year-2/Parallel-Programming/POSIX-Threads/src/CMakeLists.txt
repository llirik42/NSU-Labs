cmake_minimum_required(VERSION 3.22)
project(POSIX C)

set(CMAKE_C_STANDARD 11)

find_package(MPI)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)

add_compile_options(
        -Werror
        -Wall
        -Wextra
        -pedantic
        -Wduplicated-branches
        -Wduplicated-cond
        -Wconversion
        -Wsign-conversion
        -Wlogical-op
        -pthread
        -O0
        )

add_executable(POSIX src/main.c src/task.c src/tasks_list.c src/manager.c src/message.c src/worker.c src/tasks_response.c)

target_link_libraries(POSIX PRIVATE m)
