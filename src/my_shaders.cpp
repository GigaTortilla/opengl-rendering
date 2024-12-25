//
// Created by Martin Appel on 25.12.24.
//
#include <iostream>
#include <unistd.h>
#include <glad/glad.h>
#include <utils.h>
#include <my_shaders.h>

char *read_shader_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        std::cerr << "Failed to open shader file \"" << path << "\"\n";
        return nullptr;
    }
    const long file_size = get_file_size(f);
    const auto shader = static_cast<char *>(calloc(file_size, sizeof(char)));
    if (!shader) {
        std::cerr << "Failed to allocate shader memory" << "\n";
        free(shader);
        return nullptr;
    }
    fread(shader, sizeof(char), file_size - 1, f);
    fclose(f);
    return shader;
}

unsigned int compile_shader(const char *shader_source, GLenum shader_type) {
    unsigned int shader = glCreateShader(shader_type);
    int success;

    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char info_log[INFO_LOG_SIZE];
        glGetShaderInfoLog(shader, 0, nullptr, info_log);
        std::cerr << "ERROR::SHADER::" << shader_type << "::COMPILATION_FAILED" << info_log << "\n";
    }
    return shader;
}

unsigned int build_program(const char *vertex_shader_file_name, const char *fragment_shader_file_name) {
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int program = glCreateProgram();
    int success;

    char vertex_shader_path[PATH_SIZE];
    char fragment_shader_path[PATH_SIZE];

    getcwd(vertex_shader_path, PATH_SIZE);
    strcat(vertex_shader_path, "/../shaders/");
    strcat(vertex_shader_path, vertex_shader_file_name);

    getcwd(fragment_shader_path, PATH_SIZE);
    strcat(fragment_shader_path, "/../shaders/");
    strcat(fragment_shader_path, fragment_shader_file_name);

    char *vertex_shader_source = read_shader_file(vertex_shader_path);
    char *fragment_shader_source = read_shader_file(fragment_shader_path);

    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[INFO_LOG_SIZE];
        glGetShaderInfoLog(vertex_shader, INFO_LOG_SIZE, nullptr, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << info_log << "\n";
    }

    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[INFO_LOG_SIZE];
        glGetShaderInfoLog(fragment_shader, INFO_LOG_SIZE, nullptr, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << info_log << "\n";
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[INFO_LOG_SIZE];
        glGetProgramInfoLog(program, INFO_LOG_SIZE, nullptr, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << info_log << "\n";
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // free heap allocated memory for shader code
    free(vertex_shader_source);
    free(fragment_shader_source);

    return program;
}