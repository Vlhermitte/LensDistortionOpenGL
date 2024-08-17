#include "Camera.h"
#include "Model.h"
#include <filesystem>

Camera::Camera(int width, int height, glm::vec3 position) {
    Position = position;
    this->width = width;
    this->height = height;
}

void Camera::updateMatrix(float fovDegrees, float nearPlane, float farPlane) {
    // Initializes matrices since otherwise they will be the null matrix
    this->FOVdeg = fovDegrees;

    // Makes camera look in the right direction from the right position
    viewMatrix = glm::lookAt(Position, Position + Orientation, UpVector);
    // Adds perspective to the scene
    projectionMatrix = glm::perspective(glm::radians(fovDegrees), (float)width / height, nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projectionMatrix * viewMatrix;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
    // Exports camera matrix
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::RenderRadialDistortion(Shader &shader) {
    shader.Activate();
    glUniform3fv(glGetUniformLocation(shader.ID, "radialDistortionParams"), 1, glm::value_ptr(radialDistortionParams));
    shader.Deactivate();
}

void Camera::RenderTangentialDistortion(Shader& shader) {
    shader.Activate();
    glUniform2fv(glGetUniformLocation(shader.ID, "tangentialDistortionParams"), 1, glm::value_ptr(tangentialDistortionParams));
    shader.Deactivate();
}

void Camera::SetRadialDistortionParams(glm::vec3 distortionParams) {
    this->radialDistortionParams = distortionParams;
}

void Camera::SetTangentialDistortionParams(glm::vec2 distortionParams) {
    this->tangentialDistortionParams = distortionParams;
}

glm::vec3 Camera::GetRadialDistortionParams() {
    return radialDistortionParams;
}

glm::vec2 Camera::GetTangentialDistortionParams() {
    return tangentialDistortionParams;
}

glm::mat4 Camera::GetViewMatrix() {
    return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() {
    return projectionMatrix;
}

bool Camera::IsWireframeMode() {
    return wireframeMode;
}

void Camera::TakeScreenshot(GLFWwindow *window, const char* filename) {
    int windowsWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowsWidth, &windowHeight);
    auto *data = new unsigned char[3 * windowsWidth * windowHeight];
    glReadPixels(0, 0, windowsWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
    // Flip the image
    stbi_flip_vertically_on_write(true);
    auto filepath = "../Screenshots/" + std::string(filename);
    stbi_write_png(filepath.c_str(), windowsWidth, windowHeight, 3, data, 0);
    delete[] data;

    std::cout << "Screenshot" << filename <<"taken" << std::endl;
}

void Camera::Inputs(GLFWwindow *window) {
    handleMovement(window);
    handleMouse(window);
    handleKeyboard(window);
}

void Camera::EnableMouse() {
    mouseEnabled = true;
}

void Camera::DisableMouse() {
    mouseEnabled = false;
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
    if (mouseEnabled && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
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

    // Get image from OpenGL
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        // Save camera parameters to a file
        auto timestamp = std::chrono::system_clock::now();
        auto timestamp_c = std::chrono::system_clock::to_time_t(timestamp);
        auto screenshotName = std::string("screenshot_") + std::to_string(timestamp_c) + ".png";
        std::ofstream file;
        file.open("../Screenshots/cameras_parameters.csv", std::ios_base::app);
        // Check if header is needed
        if (file.tellp() == 0) {
            file << "x;y;z;" << "yaw;pitch;roll;" << "Up_x;Up_y;Up_z;" << "k1;k2;k3;p1;p2;" << "FOV (deg);" << "Screenshot" << std::endl;
        }
        // Go to the end of the file
        file.seekp(0, std::ios_base::end);

        // Append camera parameters to the file
        file << Position.x << ";" << Position.y << ";" << Position.z << ";"
             << Orientation.x << ";" << Orientation.y << ";" << Orientation.z << ";"
             << UpVector.x << ";" << UpVector.y << ";" << UpVector.z << ";"
             << radialDistortionParams.x << ";" << radialDistortionParams.y << ";" << radialDistortionParams.z << ";"
             << tangentialDistortionParams.x << ";" << tangentialDistortionParams.y << ";"
             << FOVdeg << ";"
             << screenshotName << std::endl;

        file.close();

        // Takes a screenshot
        TakeScreenshot(window, screenshotName.c_str());
    }
}

void Camera::GenerateDataset(GLFWwindow *window, bool pinholeCamera=false) {
    std::string directory = "../Screenshots/Dataset/";
    // Create directory if it doesn't exist
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directory(directory);
    }
    // Generate random position for the camera x = <-5, 5>, y = <1, 2>, z = <5, 7> (y is up vector)
    float x = -5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.0f - (-5.0f))));
    float y = 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0f - 1.0f)));
    float z = 5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (7.0f - 5.0f)));

    // Compute orientation of the camera for it to look at the center of the scene
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 orientation = glm::normalize(center - glm::vec3(x, y, z));

    // Generate random radial distortion parameters (between 0 and 0.1)
    float k1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1f);
    float k2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1f);
    float k3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1f);

    // Generate random tangential distortion parameters (between 0 and 0.1)
    float p1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1f);
    float p2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1f);

    // Generate random FOV (between 30 and 90 degrees)
    float fov = 30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (90.0f - 30.0f)));

    // Apply transformation to the camera
    Position = glm::vec3(x, y, z);
    Orientation = orientation;
    if (!pinholeCamera) {
        radialDistortionParams = glm::vec3(k1, k2, k3);
        tangentialDistortionParams = glm::vec2(p1, p2);
    }
    // Update camera matrix
    updateMatrix(fov, 0.1f, 100.0f);

    // Take screenshot
    auto timestamp = std::chrono::system_clock::now();
    auto timestamp_c = std::chrono::system_clock::to_time_t(timestamp);
    auto imageName = directory + std::string("img_") + std::to_string(timestamp_c) + ".png";

    TakeScreenshot(window, imageName.c_str());
}