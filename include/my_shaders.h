//
// Created by Martin Appel on 25.12.24.
//

#ifndef MY_SHADERS_H
#define MY_SHADERS_H

/**
 * Get the shader file content for compiling the shader program.
 * ATTENTION: Remember to free the memory allocated by this function after use!
 * @param path path to the shader file
 * @return content of the shader file as heap allocated char pointer
 */
char *read_shader_file(const char *path);

unsigned int compile_shader(const char *shader_source, GLenum shader_type);

unsigned int build_program(const char *vertex_shader_file_name, const char *fragment_shader_file_name);

#endif //MY_SHADERS_H
