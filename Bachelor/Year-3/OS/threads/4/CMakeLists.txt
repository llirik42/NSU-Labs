cmake_minimum_required(VERSION 3.21)
project(MyThread C)

set(exe_name ${PROJECT_NAME})
set(CMAKE_C_STANDARD 11)

add_library(corelib OBJECT
        src/mythread/mythread.c
        src/mythread/list.c
        src/mythread/my_thread_data.c
        src/mythread/gc_thread.c
        src/mythread/utils.c
)

add_executable(${exe_name} src/main.c)

target_link_libraries(${exe_name} corelib)
