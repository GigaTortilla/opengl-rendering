#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <my_shaders.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
    // initialize GLFW window
    GLFWwindow *window = init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Rendering");

    // create shader program from vertex and fragment shader
    unsigned int shader_program = build_program("shader.vert", "shader.frag");
    unsigned int shader_green = build_program("shader.vert", "green.frag");

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    float triangle1[] = {
        -0.55f, -0.3f, 0.0f,
        -0.05f, -0.3f, 0.0f,
        -0.30f,  0.3f, 0.0f
    };
    float triangle2[] = {
         0.55f, -0.3f, 0.0f,
         0.05f, -0.3f, 0.0f,
         0.30f,  0.3f, 0.0f
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glBindVertexArray(VBOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glUseProgram(shader_program);

    int vertex_color_loc = glGetUniformLocation(shader_program, "redVal");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // main rendering loop
    while (!glfwWindowShouldClose(window)) {
        process_inputs(window);

        double timeValue = glfwGetTime();
        auto red = static_cast<float>(0.5f * sin(timeValue * 5.0f) + 0.5f);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniform4f(vertex_color_loc, red, 0.1f, 0.1f, 1.0f);

        glUseProgram(shader_green);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return EXIT_SUCCESS;
}