set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "gcc")

cmake_minimum_required(VERSION 3.10)
project(ModelViewer)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

add_library(glad_library ext/glad/src/glad.c)
target_include_directories(glad_library PUBLIC ext/glad/include)

# Fetch cglm
FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG v0.8.7
)
FetchContent_MakeAvailable(cglm)

# Add all source files from the src folder
file(GLOB SOURCES src/*.c)

# Create executable
add_executable(ModelViewer ${SOURCES})
set_target_properties(ModelViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Link libraries
target_link_libraries(ModelViewer PRIVATE glfw glad_library cglm)

# Platform-specific configuration
if(WIN32)
    target_link_libraries(ModelViewer PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(ModelViewer PRIVATE "-framework OpenGL" "-framework Cocoa")
elseif(UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(ModelViewer PRIVATE ${X11_LIBRARIES})
endif()
