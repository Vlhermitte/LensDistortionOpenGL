#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "data.h"
#include "Texture.h"


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

    Shader defaultShader("../Shaders/default.vert", "../Shaders/default.frag");
    Shader lightShader("../Shaders/light.vert", "../Shaders/light.frag");

    VAO objectVAO;
    objectVAO.Bind();

    VBO objectVBO(pyramidVertices, sizeof(pyramidVertices));
    EBO objectEBO(pyramidIndices, sizeof(pyramidIndices));

    GLint posAttrib = glGetAttribLocation(defaultShader.ID, "aPos");
    GLint colorAttrib = glGetAttribLocation(defaultShader.ID, "aColor");
    GLint texCoord = glGetAttribLocation(defaultShader.ID, "aTexCoord");
    GLint normalAttrib = glGetAttribLocation(defaultShader.ID, "aNormal");

    objectVAO.LinkAttrib(objectVBO, posAttrib, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    objectVAO.LinkAttrib(objectVBO, colorAttrib, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    objectVAO.LinkAttrib(objectVBO, texCoord, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    objectVAO.LinkAttrib(objectVBO, normalAttrib, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

    objectVAO.Unbind();
    objectVBO.Unbind();
    objectEBO.Unbind();

    // Sun
    VAO sunVAO;
    sunVAO.Bind();

    VBO sunVBO(sunVertices, sizeof(sunVertices));
    EBO sunEBO(sunIndices, sizeof(sunIndices));

    GLint sunPosAttrib = glGetAttribLocation(defaultShader.ID, "aPos");

    sunVAO.LinkAttrib(sunVBO, sunPosAttrib, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    sunVAO.Unbind();
    sunVBO.Unbind();
    sunEBO.Unbind();

    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);

    glm::vec4 sunColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 sunPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 sunModel = glm::mat4(1.0f);
    sunModel = glm::translate(sunModel, sunPos);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(sunModel));
    glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));

    defaultShader.Activate();
    glUniform3fv(glGetUniformLocation(defaultShader.ID, "lightPos"), 1, glm::value_ptr(sunPos));
    glUniform4fv(glGetUniformLocation(defaultShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
    glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(objectModel));


    // Texture
    Texture crateTexture("../Textures/crate.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    crateTexture.texUnit(defaultShader, "tex0", 0);

    // Camera
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    camera.AddRadialDistortion(defaultShader, glm::vec3(0.0f, 0.0f, 0.0f));
    camera.AddTangentialDistortion(defaultShader, glm::vec2(0.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);

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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        defaultShader.Activate();
        camera.updateMatrix(60.0f, 0.1f, 100.0f);
        camera.Matrix(defaultShader, "camMatrix");

        // Texture
        crateTexture.Bind();

        // Draw Cube
        objectVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        objectVAO.Unbind();

        crateTexture.Unbind();

        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");

        sunModel = glm::rotate(sunModel, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(sunModel));

        // Draw Sun
        sunVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(sunIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        sunVAO.Unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    objectVAO.Delete();
    objectVBO.Delete();
    objectEBO.Delete();
    sunVAO.Delete();
    sunVBO.Delete();
    sunEBO.Delete();
    crateTexture.Delete();
    defaultShader.Delete();
    lightShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
