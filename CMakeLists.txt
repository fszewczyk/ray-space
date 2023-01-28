cmake_minimum_required(VERSION 3.10)

project(freengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(
    core
    src/core/vec3.cpp
    src/core/color.cpp
    src/core/ray.cpp
    src/core/utils.hpp
    )

add_library(
    shapes
    src/shapes/sphere.cpp
    src/shapes/hittableWorld.cpp
    src/shapes/hittable.hpp
    )


set(SOURCES src/main.cpp)

add_executable(freengine src/main.cpp)

target_include_directories(freengine PRIVATE src)
target_include_directories(shapes PRIVATE src)

target_link_libraries(freengine PRIVATE core shapes)





