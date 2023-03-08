cmake_minimum_required(VERSION 3.10)

project(shkyeraEngine)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_CURRENT_SOURCE_DIR}/extern/extern.cmake)

add_library(
    core
    src/core/vec3.cpp
    src/core/ray.cpp
    src/core/utils.hpp
    src/core/image.cpp
    src/core/texture.cpp
    )

add_library(
    shapes
    src/shapes/hittable.hpp
    src/shapes/sphere.cpp
    src/shapes/material.cpp
    )

add_library(
    world
    src/world/camera.cpp
    src/world/hittableWorld.cpp
    )

add_library(
    ui
    src/ui/ui.cpp
    src/ui/renderer.cpp
    src/ui/render/renderWindow.cpp
    src/ui/settings/cameraSettingsWindow.cpp
    )




set(SOURCES src/main.cpp)

add_executable(shkyera src/main.cpp)

target_include_directories(shkyera PRIVATE src)
target_include_directories(core PRIVATE src)
target_include_directories(shapes PRIVATE src)
target_include_directories(world PRIVATE src)
target_include_directories(ui PRIVATE src  ${CMAKE_BINARY_DIR}/_deps/imgui-src ${CMAKE_BINARY_DIR}/_deps/glfw-src/include)

target_link_libraries(shkyera PRIVATE core shapes world ui imgui glfw glad)
target_link_libraries(ui PRIVATE imgui glfw glad core)
target_link_libraries(core PRIVATE stb_image)




