#include <iostream>
#include <string>
#include "VAO.h"


VAO::VAO() {
#if defined(__APPLE__)
    glGenVertexArrays(1, &this->ID);
    // glGenVertexArraysAPPLE(1, &ID);
#endif
#if defined(_WIN32) || defined(__linux__)
    glGenVertexArrays(1, &ID);
#endif
    check_error_gl();
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset) {
    VBO.Bind();
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    VBO.Unbind();
    check_error_gl();
}

void VAO::Bind() const {
#if defined(__APPLE__)
    glBindVertexArray(this->ID);
    // glBindVertexArrayAPPLE(ID);
#endif
#if defined(_WIN32) || defined(__linux__)
    glBindVertexArray(ID);
#endif
    check_error_gl();
}

void VAO::Unbind() {
#if defined(__APPLE__)
    glBindVertexArray(0);
    // glBindVertexArrayAPPLE(0);
#endif
#if defined(_WIN32) || defined(__linux__)
    glBindVertexArray(0);
#endif
    check_error_gl();
}

void VAO::Delete() const {
#if defined(__APPLE__)
    glDeleteVertexArrays(1, &this->ID);
    // glDeleteVertexArraysAPPLE(1, &ID);
#endif
#if defined(_WIN32) || defined(__linux__)
    glDeleteVertexArrays(1, &ID);
#endif
    check_error_gl();
}


void VAO::check_error_gl() const {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (err) {
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                error = "UNKNOWN";
                break;
        }
        std::cerr << "OpenGL Error: " << error << " with VAO: (id=" << this->ID << ")" << std::endl;
    }

}