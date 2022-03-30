cmake_minimum_required(VERSION 3.7)
project(ichor)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(ichor main.cpp)

target_link_libraries(ichor ${SDL2_LIBRARIES})


