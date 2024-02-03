#include <iostream>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"


int main(int argc, char** argv) {

    if (!glfwInit()) {
        std::cout << "GLFW initialized" << std::endl;
        return -1;
    }

    // set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // create window
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        std::cout << "Error creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    Shader shaderProgram("../Shaders/default.vert", "../Shaders/default.frag");

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2
    };

    VAO VAO;
    VAO.Bind();

    VBO VBO(vertices, sizeof(vertices));
    EBO EBO(indices, sizeof(indices));

    GLint posAttrib = glGetAttribLocation(shaderProgram.ID, "aPos");
    GLint colAttrib = glGetAttribLocation(shaderProgram.ID, "aColor");
    VAO.LinkAttrib(VBO, posAttrib, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO.LinkAttrib(VBO, colAttrib, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        shaderProgram.Activate();
        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
