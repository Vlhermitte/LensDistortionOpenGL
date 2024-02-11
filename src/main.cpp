#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Shader.h"
#include "Camera.h"
#include "data.h"
#include "Mesh.h"
#include "Model.h"

std::vector<Model> initModels() {
    std::vector<Model> models;
    // RaceCar
    Model raceCar("../Resources/Models/RaceCar/RaceCar.obj");
    raceCar.setPosition(glm::vec3(0.5f, -0.5f, 0.5f));
    raceCar.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    models.emplace_back(raceCar);

    return models;
}

int main(int argc, char** argv) {

    if (!glfwInit()) {
        std::cout << "Error initializing glfw" << std::endl;
        return -1;
    }

    // set up glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        std::cout << "Error creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // set up glew
    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew" << std::endl;
        return -1;
    }

    // Load planksTextures
    Texture planksTextures[] {
            Texture("../Resources/Textures/planks/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
            Texture("../Resources/Textures/planks/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    Shader defaultShader("../Shaders/default.vert", "../Shaders/default.frag");
    Shader lightShader("../Shaders/light.vert", "../Shaders/light.frag");
    Shader shadowMapShader("../Shaders/shadowMap.vert", "../Shaders/shadowMap.frag"); // Not used yet

    /// Objects
    // floor
    std::vector<Vertex> floorVerts(floorVertices, floorVertices + sizeof(floorVertices) / sizeof(Vertex));
    std::vector<GLuint> floorInd(floorIndices, floorIndices + sizeof(floorIndices) / sizeof(GLuint));
    std::vector<Texture> tex(planksTextures, planksTextures + sizeof(planksTextures) / sizeof(Texture));
    Mesh floor(floorVerts, floorInd, tex);

    // Sun
    std::vector<Vertex> sunVerts(sunVertices, sunVertices + sizeof(sunVertices) / sizeof(Vertex));
    std::vector<GLuint> sunInd(sunIndices, sunIndices + sizeof(sunIndices) / sizeof(GLuint));
    Mesh sun(sunVerts, sunInd, tex);

    glm::vec3 objectPos = glm::vec3(0.0f, -0.5f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);

    glm::vec4 sunColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 sunPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 sunModel = glm::mat4(1.0f);
    sunModel = glm::translate(sunModel, sunPos);

    defaultShader.Activate();
    glUniform3fv(glGetUniformLocation(defaultShader.ID, "lightPos"), 1, glm::value_ptr(sunPos));
    glUniform4fv(glGetUniformLocation(defaultShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
    glUniform1i(glGetUniformLocation(defaultShader.ID, "usePointLight"), true);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "useDirectionalLight"), false);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "useSpotLight"), true);
    defaultShader.Deactivate();

    lightShader.Activate();
    glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
    lightShader.Deactivate();

    // Models
    std::vector<Model> models = initModels();

    // Camera
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    camera.AddRadialDistortion(defaultShader, glm::vec3(0.0f, 0.3f, 0.0f));
    camera.AddTangentialDistortion(defaultShader, glm::vec2(0.0f, 0.0f));

    // Time
    float previousTime = 0.0f;
    float currentTime = 0.0f;
    float deltaTime = 0.0f;
    unsigned int framesCounter = 0;

    while (!glfwWindowShouldClose(window)) {

        currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        framesCounter++;
        if (deltaTime >= 1.0f) {
            std::string fps = std::to_string((1.0 / deltaTime) * framesCounter);
            std::string ms = std::to_string(deltaTime * 1000.0 / framesCounter);
            std::string title = "OpenGL Project | FPS: " + fps + " | " + ms + " (ms)";
            glfwSetWindowTitle(window, title.c_str());
            previousTime = currentTime;
            framesCounter = 0;
        }
        if (deltaTime >= 1.0f / 30.0f) {
            // Putting the Inputs handling here, to avoid inputs depending on the frame rate
            camera.Inputs(window);
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        camera.updateMatrix(60.0f, 0.1f, 100.0f);
        camera.Matrix(defaultShader, "camMatrix");

        // Draw Meshes
        floor.Draw(defaultShader, camera, objectModel);
        sun.Draw(lightShader, camera, sunModel);

        // Draw models
        for (auto & model : models) {
            model.Draw(defaultShader, camera);
        }

        // Swap buffers and GLFW poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    defaultShader.Delete();
    lightShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
