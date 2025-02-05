//
// Created by Martin Appel on 09.01.25.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "GLFW/glfw3.h"

#define MOVEMENT_SPEED 2.5f
#define MOUSE_SENSITIVITY 0.1f
#define FOV 60.0f

class Camera {
public:
    glm::vec3 position{};
    glm::vec3 front;
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 world_up{};

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;
    float fov;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 get_view_mat() const;
    void move_camera(float delta_time, GLFWwindow *window);
    void scroll_fov(float y_offset);
    void mouse_movement(float xoffset, float yoffset, bool constrainPitch = true);

private:
    void update_camera_vectors();
};



#endif //CAMERA_H
