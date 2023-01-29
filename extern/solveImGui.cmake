FetchContent_Declare(      
    imgui                  
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG "259560aa26476566801ed46006480b7f1d59c557"
    GIT_PROGRESS TRUE
)
 
FetchContent_MakeAvailable(imgui)
 
set(IMGUI_SOURCE
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_demo.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_draw.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_tables.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_widgets.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_glfw.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC
    ${IMGUI_SOURCE}
)

target_include_directories(imgui PUBLIC "${CMAKE_BINARY_DIR}/_deps/imgui-src")
target_link_libraries(imgui PRIVATE glfw)

