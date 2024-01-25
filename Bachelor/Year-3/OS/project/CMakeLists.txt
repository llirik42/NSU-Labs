cmake_minimum_required(VERSION 3.20)
project(Proxy)

set(exe_name ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
        argparse
        URL https://github.com/p-ranav/argparse/archive/refs/tags/v2.9.zip
)
FetchContent_MakeAvailable(argparse)

add_library(corelib OBJECT
        src/utils/cerror.cpp
        src/utils/mutex.cpp
        src/utils/rwlock.cpp
        src/utils/thread.cpp
        src/utils/socket_like_selector.cpp

        src/utils_linux/errors.cpp
        src/utils_linux/socket_actions.cpp
        src/utils_linux/socket_selector.cpp
        src/utils_linux/transmitter.cpp

        src/networking/connection.cpp
        src/networking/server.cpp

        src/http/http_utils.cpp
        src/http/http_request_parser.cpp
        src/http/request_analyzer.cpp
        src/http/http_response_creator.cpp
        src/http/http_response_parser.cpp

        src/cache/cache.cpp

        src/logging/std_logger.cpp
        src/logging/file_logger.cpp
        src/logging/synchronous_logger.cpp

        src/work/client_context.cpp
        src/work/destination_context.cpp
        src/work/worker_transmitter.cpp
        src/work/worker.cpp
        src/work/thread_pool.cpp

        src/proxy/proxy.cpp
)

target_compile_options(corelib PUBLIC -O3)

add_executable(${exe_name} src/main.cpp)

target_link_libraries(${exe_name} PRIVATE corelib argparse Threads::Threads)
