//
// Created by Martin Appel on 26.12.24.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <my_shaders.h>
#include <render_programs.h>
#include <utils.h>

int triangles() {
    // initialize GLFW window
    GLFWwindow *window = init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Rendering");

    // create shader program from vertex and fragment shader
    unsigned int shader_program = build_program("tri.vert", "tri.frag");
    unsigned int shader_green = build_program("tri.vert", "green_tri.frag");

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

    // create vertex buffer objects and vertex array objects
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // first triangle
    // 1. bind the VAO
    // 2. bind VBO and specify vertex buffer data (the object to render)
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
    // 3. specify the values / attributes sent to the shader (how the VBO is organized)
    // index - gets incremented for each layout location / variable in shader
    // size - number of values (3 for vec3 and so on)
    // stride - amount of occupied memory (distance between attributes)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    // enable specified attributes for following index
    glEnableVertexAttribArray(0);
    // unbind VAO
    glBindVertexArray(0);

    // second triangle
    glBindVertexArray(VBOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glUseProgram(shader_program);

    // getting the location of the vertex color uniform in the fragment shader
    int vertex_color_loc = glGetUniformLocation(shader_program, "redVal");

    /**
     * Wireframe Mode
     * GL_FILL - filled shapes
     * GL_LINE - wireframe mode
     */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // main rendering loop
    while (!glfwWindowShouldClose(window)) {
        process_inputs(window);

        double timeValue = glfwGetTime();
        auto red = static_cast<float>(0.5f * sin(timeValue * 5.0f) + 0.5f);

        // background color^:w
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // first triangle
        glUseProgram(shader_program);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniform4f(vertex_color_loc, red, 1.0f - red, 0.1f, 1.0f);

        // second triangle
        glUseProgram(shader_green);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup after use
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shader_program);
    glDeleteProgram(shader_green);

    glfwTerminate();
    return EXIT_SUCCESS;
}