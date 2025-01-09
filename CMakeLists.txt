cmake_minimum_required(VERSION 3.30)
project(opengl_rendering)

set(CMAKE_CXX_STANDARD 23)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)

add_executable(opengl_rendering src/main.cpp
        src/render_programs.cpp include/render_programs.h)
add_library(opengl_rendering_lib SHARED
        src/utils.cpp include/utils.h
        src/my_shaders.cpp include/my_shaders.h
        src/glad.c
        src/Camera.cpp
        include/Camera.h)

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw-3.4)
add_subdirectory(${PROJECT_SOURCE_DIR}/glm-1.0.1)

include_directories(${PROJECT_SOURCE_DIR}/glfw-3.4/include)
include_directories(${PROJECT_SOURCE_DIR}/glm-1.0.1)
target_include_directories(opengl_rendering PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(opengl_rendering_lib PRIVATE ${PROJECT_SOURCE_DIR}/include)

if (WIN32)
    target_link_libraries(opengl_rendering_lib glfw ${GLFW_LIBRARIES} glm::glm)
else ()
    find_library(GLFW3 libglfw.dylib PATHS lib)
    target_link_libraries(opengl_rendering_lib ${GLFW3})
endif ()

target_link_libraries(opengl_rendering opengl_rendering_lib)