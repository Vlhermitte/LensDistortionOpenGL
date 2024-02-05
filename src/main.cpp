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

    Shader shaderProgram("../Shaders/default.vert", "../Shaders/default.frag");

    VAO VAO;
    VAO.Bind();

    VBO VBO(squareVertices, sizeof(squareVertices));
    EBO EBO(squareIndices, sizeof(squareIndices));

    GLint posAttrib = glGetAttribLocation(shaderProgram.ID, "aPos");
    GLint colorAttrib = glGetAttribLocation(shaderProgram.ID, "aColor");
    GLint texCoord = glGetAttribLocation(shaderProgram.ID, "aTexCoord");

    VAO.LinkAttrib(VBO, posAttrib, 3, GL_FLOAT,  8 * sizeof(float), (void*)0);
    // VAO.LinkAttrib(VBO, colorAttrib, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, texCoord, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    // Texture
    Texture crateTexture("../Textures/crate.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    crateTexture.texUnit(shaderProgram, "tex0", 0);

    // Camera
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 1.0f));
    camera.AddRadialDistortion(shaderProgram, glm::vec3(0.0f, 0.5f, 0.0f));
    camera.AddTangentialDistortion(shaderProgram, glm::vec2(0.0f, 0.0f));

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // draw triangle
        shaderProgram.Activate();

        camera.Matrix(60.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        // Texture
        crateTexture.Bind();

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        crateTexture.Unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    crateTexture.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
