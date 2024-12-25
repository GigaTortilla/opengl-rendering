//
// Created by Martin Appel on 22.12.24.
//

#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <GLFW/glfw3.h>

#define INFO_LOG_SIZE 1024
#define PATH_SIZE 1024

void process_inputs(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/**
 * Get the size of a file by getting the last position of file stream + 1.
 * Requires the file to be opened in binary mode.
 * @param f input file stream in binary mode
 * @return size of the file in Bytes
 */
long get_file_size(FILE *f);

GLFWwindow *init_window(int width, int height, const char *title);

#endif //UTILS_H
