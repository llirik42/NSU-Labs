cmake_minimum_required(VERSION 3.26)
project(UserThreads C)

set(CMAKE_C_STANDARD 11)

add_library(corelib OBJECT
        uthread/uthread.c
        uthread/uthread_data.c
        uthread/utils.c
        uthread/uthread_queue.c
        uthread/scheduler.c
)

add_executable(
        UserThreads
        main.c
)

target_compile_options(corelib PUBLIC -O0)

target_link_libraries(UserThreads corelib)
