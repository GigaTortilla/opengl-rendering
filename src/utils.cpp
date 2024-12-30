//
// Created by Martin Appel on 22.12.24.
//

#include <cstdio>
#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void process_inputs(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

long get_file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f) + 1;
    rewind(f);
    return file_size;
}

void set_window_icon(GLFWwindow *window, const char *path) {
    GLFWimage icon;
    int width, height, channels;
    icon.pixels = stbi_load(path, &width, &height, &channels, 0);
    if (icon.pixels) {
        icon.width = width;
        icon.height = height;
        glfwSetWindowIcon(window, 1, &icon);
    } else {
        std::cerr << "Failed to load icon at path: " << path << "\n";
    }
    stbi_image_free(icon.pixels);
}

GLFWwindow *init_window(int width, int height, const char *title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return nullptr;
    }

    //glEnable(GL_DEPTH_TEST);
    return window;
}