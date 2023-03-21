FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(implot)

set(IMPLOT_SOURCE
    ${CMAKE_BINARY_DIR}/_deps/implot-src/implot.cpp
    ${CMAKE_BINARY_DIR}/_deps/implot-src/implot_demo.cpp
    ${CMAKE_BINARY_DIR}/_deps/implot-src/implot_items.cpp
)

add_library(implot STATIC
    ${IMPLOT_SOURCE}
)

target_include_directories(implot PUBLIC "${CMAKE_BINARY_DIR}/implot-src")
target_link_libraries(implot PRIVATE glfw imgui)