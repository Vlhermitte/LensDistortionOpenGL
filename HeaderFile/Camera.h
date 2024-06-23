//
// Created by Valentin on 05/02/2024.
//

#ifndef OPENGLPROJECT_CAMERA_H
#define OPENGLPROJECT_CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <stb/stb_image_write.h>
#include <chrono>

#include "Shader.h"


class Camera {
private:
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    glm::vec3 radialDistortionParams = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 tangentialDistortionParams = glm::vec2(0.0f, 0.0f);
    int width;
    int height;
    float FOVdeg{};
    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;
    float speed = 0.01f;
    float sensitivity = 100.0f;
    bool wireframeMode = false;
    bool mouseEnabled = true;

    void handleMovement(GLFWwindow* window);
    void handleMouse(GLFWwindow* window);
    void handleKeyboard(GLFWwindow* window);
public:
    glm::vec3 Position{};
    Camera(int width, int height, glm::vec3 position);
    void updateMatrix(float fovDegrees, float nearPlane, float farPlane);
    void Matrix(Shader& shader, const char* uniform);
    void RenderRadialDistortion(Shader& shader);
    void RenderTangentialDistortion(Shader& shader);

    // Setter
    void setRadialDistortionParams(glm::vec3 distortionParams);
    void setTangentialDistortionParams(glm::vec2 distortionParams);

    // Getter
    glm::vec3 getRadialDistortionParams();
    glm::vec2 getTangentialDistortionParams();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    bool IsWireframeMode();

    void TakeScreenshot(GLFWwindow *window, const char* filename);
    void Inputs(GLFWwindow* window);
    void EnableMouse();
    void DisableMouse();
};


#endif //OPENGLPROJECT_CAMERA_H
