//
// Created by Martin Appel on 26.12.24.
//

// external dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Standard Library includes
#include <iostream>
#include <cmath>

// custom header files
#include <my_shaders.h>
#include <render_programs.h>
#include <utils.h>
#include <Camera.h>

// stb image for image loading (textures)
// Definition required before including stb image to successfully build
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int cubes() {
    // initialize GLFW window
    GLFWwindow *window = init_window_3d(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Rendering");

    double last_frame = 0.0;

    const GLuint tex_crate = load_texture("../textures/container.jpg");

    const GLuint shader_program = build_program("cubes.vert", "cubes.frag");

    constexpr GLsizei stride = 5;
    constexpr float cube[] = {
        // model coords		 // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    const glm::vec3 cube_positions[] = {
        {  0.0f,  0.0f,  0.0f },
        {  2.0f,  5.0f, -15.0f },
        { -1.5f, -2.2f, -2.5f },
        { -3.8f, -2.0f, -12.3f },
        {  2.4f, -0.4f, -3.5f },
        { -1.7f,  3.0f, -7.5f },
        {  1.3f, -2.0f, -2.5f },
        {  1.5f,  2.0f, -2.5f },
        {  1.5f,  0.2f, -1.5f },
        { -1.3f,  1.0f, -1.5f }
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // created VAO for the cube model
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // unbind VAO
    glBindVertexArray(0);

    glUseProgram(shader_program);

    // get uniform locations in memory
    const GLint model_location = glGetUniformLocation(shader_program, "model");
    const GLint view_location = glGetUniformLocation(shader_program, "view");
    const GLint projection_location = glGetUniformLocation(shader_program, "projection");

    // prepare shader for texture input
    glUniform1i(glGetUniformLocation(shader_program, "texture"), 0);

    /**
     * Wireframe Mode
     * GL_FILL - filled shapes
     * GL_LINE - wireframe mode
     */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Camera cam = Camera(0.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

    // main rendering loop
    while (!glfwWindowShouldClose(window)) {
        // Handling time dependent actions using the time difference between the last 2 rendered frames
        const double time_value = glfwGetTime();
        double delta_time = time_value - last_frame;
        last_frame = time_value;

        process_inputs(window);
        cam.move_camera(static_cast<float>(delta_time), window);

        // background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // set color and depth buffer bit --> depth buffer for only showing the vertices facing the camera
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // get texture for container
        glBindTexture(GL_TEXTURE_2D, tex_crate);

        glUseProgram(shader_program);

        // get window aspect ratio
        int screen_width, screen_height;
        glfwGetWindowSize(window, &screen_width, &screen_height);
        const float aspect_ratio = static_cast<float>(screen_width) / static_cast<float>(screen_height);

        // setting the view and projection matrix every frame
        // could be done outside the main render loop
        auto view = cam.get_view_mat();
        auto projection = glm::perspective(
            // field of view
            cam.fov,
            aspect_ratio,
            // render depth
            0.1f,
            100.0f
        );
        glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);

        // render cubes
        glBindVertexArray(VAO);
        for (GLuint i = 0; i < 10; i++) {
            auto model = glm::mat4(1.0f);
            // move cubes to their positions
            model = glm::translate(model, cube_positions[i]);
            // rotate cubes differently
            float angle;
            if (i % 3 == 0) angle = 25.0f * static_cast<float>(time_value);
            else angle = 20.0f * static_cast<float>(i);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return EXIT_SUCCESS;
}

int math_example() {
    // initialize GLFW window
    GLFWwindow *window = init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Rendering");

    GLuint tex_face = load_texture("../textures/awesomeface.png", true);
    GLuint tex_crate = load_texture("../textures/container.jpg");

    int width, height, nrChannels;
    glBindTexture(GL_TEXTURE_2D, tex_crate);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load image\n";
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    stbi_set_flip_vertically_on_load(false);

    GLuint shader_program = build_program("transform.vert", "transform.frag");

    constexpr float vertices[] = {
        // vertex coords    // texture
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left
    };
    const unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glUseProgram(shader_program);

    const GLint transform_location = glGetUniformLocation(shader_program, "transform");
    const GLint blend_location = glGetUniformLocation(shader_program, "blend");

    // prepare shader for texture input
    glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "texture1"), 1);

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
        const double time_value = glfwGetTime();

        auto trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f * sin(time_value * 2.0f), 0.5f * sin(time_value * 3.0f), 0.0f));
        trans = glm::rotate(trans, glm::radians(static_cast<float>(time_value * 25.0f)), glm::vec3(0.0f, 0.0f, 1.0f));

        // background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render container
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_crate);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex_face);
        glBindVertexArray(VAO);
        glUseProgram(shader_program);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        // send uniforms to shader program
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform1f(blend_location, 0.2f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return EXIT_SUCCESS;
}

int triangles() {
    // initialize GLFW window
    GLFWwindow *window = init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Rendering");

    // create shader program from vertex and fragment shader
    unsigned int shader_program = build_program("tri_flip.vert", "tri_variable.frag");
    unsigned int shader_green = build_program("tri_flip.vert", "tri_green.frag");
    unsigned int shader_tex = build_program("tri_tex.vert", "tri_tex.frag");
    unsigned int shader_crate = build_program("tri_crate_tex.vert", "tri_crate_tex.frag");

    ////////////////
    /// Textures ///
    ////////////////
    GLuint texture[3];

    texture[0] = load_texture("../textures/wall.jpg");
    texture[1] = load_texture("../textures/container.jpg");
    texture[2] = load_texture("../textures/awesomeface.png", true);

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
    float crate[] = {
        // positions          // colors           // texture coords
       -0.45f, -0.45f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
       -0.45f, -0.95f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
       -0.95f, -0.95f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
       -0.95f, -0.45f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int crate_indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    double last_frame = 0.0;

    // create vertex buffer objects and vertex array objects
    unsigned int VBOs[4], VAOs[4], EBO;
    glGenVertexArrays(4, VAOs);
    glGenBuffers(4, VBOs);
    glGenBuffers(1, &EBO);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // fourth object (crate)
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crate), crate, GL_STATIC_DRAW);
    // bind element buffer object for indices of the vertices (triangle render order)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crate_indices), crate_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(0));
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

    glUseProgram(shader_crate);
    GLint blend_loc = glGetUniformLocation(shader_crate, "blend");
    glUniform1i(glGetUniformLocation(shader_crate, "texture0"), 0);
    glUniform1i(glGetUniformLocation(shader_crate, "texture1"), 1);

    /**
     * Wireframe Mode
     * GL_FILL - filled shapes
     * GL_LINE - wireframe mode
     */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float delta_blend = 0.5f;

    // main rendering loop
    while (!glfwWindowShouldClose(window)) {
        process_inputs(window);

        // Handling time dependent actions using the time difference between the last 2 rendered frames
        double time_value = glfwGetTime();
        double delta_time = time_value - last_frame;
        last_frame = time_value;

        auto red = static_cast<float>(0.5f * sin(time_value * 5.0f) + 0.5f);
        auto offset = static_cast<float>(sin(time_value * 2.0f) * 0.4f);
        delta_blend += static_cast<float>((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) ? -0.5f * delta_time :
            (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) ? 0.5f * delta_time : 0.0f);

        // background color
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
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glUseProgram(shader_tex);
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // crate with texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glUseProgram(shader_crate);
        glBindVertexArray(VAOs[3]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glUniform1f(blend_loc, delta_blend);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup after use
    glDeleteVertexArrays(4, VAOs);
    glDeleteBuffers(4, VBOs);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);
    glDeleteProgram(shader_green);
    glDeleteProgram(shader_tex);
    glDeleteProgram(shader_crate);

    glfwTerminate();
    return EXIT_SUCCESS;
}