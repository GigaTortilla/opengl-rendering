cmake_minimum_required(VERSION 3.30)
project(opengl_rendering)

set(CMAKE_CXX_STANDARD 23)

add_executable(opengl_rendering src/main.cpp)
add_library(opengl_rendering_lib SHARED
        src/utils.cpp include/utils.h
        src/my_shaders.cpp include/my_shaders.h
        src/render_programs.cpp include/render_programs.h
        src/glad.c)

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw-3.4)
include_directories(${PROJECT_SOURCE_DIR}/glfw-3.4/include)
target_include_directories(opengl_rendering PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(opengl_rendering_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (WIN32)
    target_link_libraries(opengl_rendering_lib glfw ${GLFW_LIBRARIES})
else ()
    find_library(GLFW3 libglfw.dylib PATHS lib)
    target_link_libraries(opengl_rendering_lib ${GLFW3})
endif ()

target_link_libraries(opengl_rendering opengl_rendering_lib)