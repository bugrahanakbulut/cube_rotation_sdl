cmake_minimum_required(VERSION 3.30)
project(cube_rotation C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Modules)

set(SDL2_PATH "${CMAKE_SOURCE_DIR}/Modules/SDL2-2.30.9/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_PATH}/include)

add_executable(cube_rotation main.c)

target_link_libraries(cube_rotation ${SDL2_LIBRARY})