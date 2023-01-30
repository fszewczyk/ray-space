set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(      
    glfw                  
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "8f470597d625ae28758c16b4293dd42d63e8a83a"
    GIT_PROGRESS TRUE
)
 
FetchContent_MakeAvailable(glfw)

target_include_directories(glfw PUBLIC ${CMAKE_BINARY_DIR}/_deps/glfw-src/include)
