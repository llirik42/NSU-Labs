cmake_minimum_required(VERSION 3.26)
project(MyThread C)

set(CMAKE_C_STANDARD 11)

add_executable(
        MyThread
        src/main.c
        src/mythread/mythread.c
        src/mythread/list.c
        src/mythread/my_thread_data.c
        src/mythread/gc_thread.c
        src/mythread/utils.c
)
