//
// Created by Martin Appel on 22.12.24.

#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <GLFW/glfw3.h>

#define INFO_LOG_SIZE 1024
#define PATH_SIZE 1024

/**
 * checking for ESC key presses to close the window
 * @param window window to be closed on input
 */
void process_inputs(GLFWwindow *window);

/**
 * callback function for window resizing
 * @param window window attached to the callback function
 * @param width new window width
 * @param height new window height
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/**
 * Get the size of a file by getting the last position of file stream + 1.
 * Requires the file to be opened in binary mode.
 * @param f input file stream in binary mode
 * @return size of the file in Bytes
 */
long get_file_size(FILE *f);

/**
 * Loads an image as a texture into memory
 * @param path path to the image file
 * @param flipped flag determines if the image should be processed with flipped y axis
 * @return texture location in memory, can be passed to the shader
 */
GLuint load_texture(const char *path, bool flipped);

/**
 * Sets the icon of the corresponding window process
 * @param window window for which the icon should be set
 * @param path path to the icon image file
 */
void set_window_icon(GLFWwindow *window, const char *path);

/**
 * Initializes a GLFW window object
 * @param width initial window width
 * @param height initial window height
 * @param title name of the window process
 * @return opaque window object
 */
GLFWwindow *init_window(int width, int height, const char *title);

#endif //UTILS_H
