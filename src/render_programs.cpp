//
// Created by Martin Appel on 26.12.24.
//

// external dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Library includes
#include <iostream>
#include <cmath>

// custom header files
#include <my_shaders.h>
#include <render_programs.h>
#include <utils.h>

// stb image for image loading (textures)
// Definition required before including stb image to successfully build
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int triangles() {
    // initialize GLFW window
    GLFWwindow *window = init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Rendering");

    // create shader program from vertex and fragment shader
    unsigned int shader_program = build_program("tri_flip.vert", "tri_variable.frag");
    unsigned int shader_green = build_program("tri_flip.vert", "green_tri.frag");
    unsigned int shader_tex = build_program("tri_tex.vert", "tri_tex.frag");

    ////////////////
    /// Textures ///
    ////////////////
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // texture parameter (wrapping and filtering) on the currently bound texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate texture from image
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load image\n";
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    // triangle coordinates
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
    float triangle3[] = {
        // vertex position  // color values   // texture coords
        -0.3f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.3f, -0.7f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.0f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f
    };
    double last_frame = 0.0;

    // create vertex buffer objects and vertex array objects
    unsigned int VBOs[3], VAOs[3];
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);

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
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // third triangle
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle3), triangle3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glUseProgram(shader_program);

    // getting the location of the vertex color uniform in the fragment shader
    int vertex_color_loc = glGetUniformLocation(shader_program, "redVal");
    int vertex_offset_loc = glGetUniformLocation(shader_program, "offset");

    /**
     * Wireframe Mode
     * GL_FILL - filled shapes
     * GL_LINE - wireframe mode
     */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // main rendering loop
    while (!glfwWindowShouldClose(window)) {
        process_inputs(window);

        // Handling time dependent actions using the time difference between the last 2 rendered frames
        double time_value = glfwGetTime();
        double delta_time = time_value - last_frame;
        last_frame = time_value;

        auto red = static_cast<float>(0.5f * sin(time_value * 5.0f) + 0.5f);
        auto offset = static_cast<float>(sin(time_value * 2.0f) * 0.4f);

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

        glUniform1f(vertex_offset_loc, offset);

        // third triangle
        glBindTexture(GL_TEXTURE_2D, texture);
        glUseProgram(shader_tex);
        glBindVertexArray(VAOs[2]);
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