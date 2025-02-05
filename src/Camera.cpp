//
// Created by Martin Appel on 09.01.25.
//

#include "Camera.h"

#include <glm/glm.hpp>

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(MOVEMENT_SPEED), mouse_sensitivity(MOUSE_SENSITIVITY), fov(glm::radians(FOV)) {
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;

    update_camera_vectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(MOVEMENT_SPEED), mouse_sensitivity(MOUSE_SENSITIVITY), fov(glm::radians(FOV)) {
    this->position = glm::vec3(posX, posY, posZ);
    this->world_up = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;

    update_camera_vectors();
}

glm::mat4 Camera::get_view_mat() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::move_camera(float delta_time, GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * movement_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * movement_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(front, up)) * movement_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(front, up)) * movement_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += world_up * movement_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        position -= world_up * movement_speed * delta_time;
}


void Camera::update_camera_vectors() {
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(new_front);

    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
