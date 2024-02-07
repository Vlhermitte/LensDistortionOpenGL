#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
    Position = position;
    this->width = width;
    this->height = height;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, UpVector);

    view = glm::lookAt(Position, Position + Orientation, UpVector);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::AddRadialDistortion(Shader &shader, glm::vec3 distParams) {
    glUniform3fv(glGetUniformLocation(shader.ID, "radialDistortionParams"), 1, glm::value_ptr(distParams));
}

void Camera::AddTangentialDistortion(Shader& shader, glm::vec2 distParams) {
    glUniform2fv(glGetUniformLocation(shader.ID, "tangentialDistortionParams"), 1, glm::value_ptr(distParams));
}

void Camera::Inputs(GLFWwindow *window) {
    // Player Movements (QUERTY)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Position -= speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Position -= glm::normalize(glm::cross(Orientation, UpVector)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Position += glm::normalize(glm::cross(Orientation, UpVector)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Position += UpVector * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        Position -= UpVector * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.05f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
        speed = 0.01f;
    }

    // Wireframe mode
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (!wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframeMode = true;
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframeMode = false;
        }
    }

}