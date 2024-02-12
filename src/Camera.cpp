#include "Camera.h"
// if data is not included, include it
#ifndef OPENGLPROJECT_DATA_H
#define OPENGLPROJECT_DATA_H
#include "data.h"
#endif


Camera::Camera(int width, int height, glm::vec3 position) {
    Position = position;
    this->width = width;
    this->height = height;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    // Initializes matrices since otherwise they will be the null matrix
    this->FOVdeg = FOVdeg;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, UpVector);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
    // Exports camera matrix
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::AddRadialDistortion(Shader &shader, glm::vec3 distParams) {
    shader.Activate();
    glUniform3fv(glGetUniformLocation(shader.ID, "radialDistortionParams"), 1, glm::value_ptr(distParams));
}

void Camera::AddTangentialDistortion(Shader& shader, glm::vec2 distParams) {
    shader.Activate();
    glUniform2fv(glGetUniformLocation(shader.ID, "tangentialDistortionParams"), 1, glm::value_ptr(distParams));
}

void Camera::Inputs(GLFWwindow *window) {
    handleMovement(window);
    handleMouse(window);
    handleKeyboard(window);
}

void Camera::handleMovement(GLFWwindow *window) {
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
}

void Camera::handleMouse(GLFWwindow *window) {
    // Mouse Movements
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick) {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, UpVector)));

        // Decides whether the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, UpVector) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
            Orientation = newOrientation;
        }

        // Rotates the Orientation left and right
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), UpVector);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}

void Camera::handleKeyboard(GLFWwindow *window) {
    // Escape button
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
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

    // Get image from OpenGL and use it with OpenCV
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        int windowsWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowsWidth, &windowHeight);
        unsigned char *data = new unsigned char[3 * windowsWidth * windowHeight];
        glReadPixels(0, 0, windowsWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
        cv::Mat image(windowHeight, windowsWidth, CV_8UC3, data);
        cv::flip(image, image, 0);

        // Apply radial distortion
        cv::Mat undistorted;

        // Set fx and fy using FOV
        float fx = (windowsWidth) / (2.0 * tan(glm::radians(FOVdeg / 2.0)));
        float fy = (windowHeight) / (2.0 * tan(glm::radians(FOVdeg / 2.0)));
        cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << fx, 0, windowsWidth / 2, 0, fy, windowHeight / 2, 0, 0, 1);
        cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << 0.0, 0.2, 0.0, 0.0, 0.0);
        cv::undistort(image, undistorted, cameraMatrix, distCoeffs);
        // fit the image to the window
        cv::resize(undistorted, undistorted, cv::Size(800, 600));
        cv::imwrite("../Screenshots/screenshot.png", undistorted);

        delete[] data;
    }
}