#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "data.h"


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
    GLFWwindow* window = glfwCreateWindow(640 * 2, 480 * 2, "Hello World", NULL, NULL);
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

    VBO VBO(cubeVertices, sizeof(cubeVertices));
    EBO EBO(cubeIndices, sizeof(cubeIndices));

    GLint posAttrib = glGetAttribLocation(shaderProgram.ID, "aPos");
    GLint colAttrib = glGetAttribLocation(shaderProgram.ID, "aColor");
    // GLint normalAttrib = glGetAttribLocation(shaderProgram.ID, "aNormal");
    VAO.LinkAttrib(VBO, posAttrib, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO.LinkAttrib(VBO, colAttrib, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // VAO.LinkAttrib(VBO, normalAttrib, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        shaderProgram.Activate();
        VAO.Bind();
        glDrawElements(GL_TRIANGLES, cubeIndicesSize * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
