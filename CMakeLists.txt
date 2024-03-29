cmake_minimum_required(VERSION 3.10)

project(shkyeraEngine)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIE -no-pie")
endif()

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
    src/shapes/sphere.cpp
    src/shapes/material.cpp
    )

add_library(
    world
    src/world/camera.cpp
    src/world/visibleWorld.cpp
    )

add_library(
    ui
    src/ui/ui.cpp
    src/ui/renderer.cpp
    src/ui/render/renderWindow.cpp
    src/ui/plot/plotWindow.cpp
    src/ui/plot/plotViewTab.cpp
    src/ui/settings/cameraSettingsWindow.cpp
    src/ui/settings/worldSettingsWindow.cpp
    src/ui/settings/planetSettingsWindow.cpp
    src/ui/settings/exportSettingsWindow.cpp
    )




set(SOURCES src/main.cpp)

add_executable(shkyera src/main.cpp)

target_include_directories(shkyera PRIVATE src)
target_include_directories(core PRIVATE src ${CMAKE_BINARY_DIR}/_deps/glfw-src/include)
target_include_directories(shapes PRIVATE src)
target_include_directories(world PRIVATE src)
target_include_directories(ui PRIVATE src  
    ${CMAKE_BINARY_DIR}/_deps/imgui-src 
    ${CMAKE_BINARY_DIR}/_deps/implot-src 
    ${CMAKE_BINARY_DIR}/_deps/glfw-src/include)

target_link_libraries(shkyera PRIVATE core shapes world ui imgui glfw glad)
target_link_libraries(ui PRIVATE world imgui implot glfw glad core)
target_link_libraries(core PRIVATE stb_image glfw glad)




