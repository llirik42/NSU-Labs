cmake_minimum_required(VERSION 3.23)
project(OpenCV)

set(OpenCV_DIR "C:\\OpenCV")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:\\OpenCV\\x64\\mingw\\lib")

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(OpenCV main.cpp)

set(OpenCV_LIBS opencv_core opencv_highgui)

target_link_libraries(OpenCV ${OpenCV_LIBS})