cmake_minimum_required(VERSION 3.30)
project(opengl_rendering)

set(CMAKE_CXX_STANDARD 23)

add_executable(opengl_rendering src/main.cpp src/glad.c)
add_library(opengl_rendering_lib STATIC
        src/utils.cpp include/utils.h
        src/my_shaders.cpp include/my_shaders.h)

find_library(GLFW3 libglfw.dylib PATHS lib)

target_link_libraries(opengl_rendering ${GLFW3} opengl_rendering_lib)
target_include_directories(opengl_rendering PUBLIC include)
target_include_directories(opengl_rendering_lib PUBLIC include)