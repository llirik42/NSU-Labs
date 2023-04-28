cmake_minimum_required(VERSION 3.20)
project (GFX C)

set(CMAKE_C_STANDARD 99)

set(SDL2_INCLUDE_DIR C:/graphics/SDL/include)
set(SDL2_LIB_DIR C:/graphics/SDL/lib/x86)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(GFX src/main.c src/generation.c src/generations_container.c src/utils.c  src/drawer.c)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
