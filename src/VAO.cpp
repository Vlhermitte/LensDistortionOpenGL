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
    CHECK_GL_ERROR();
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset) {
    VBO.Bind();
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    VBO.Unbind();
    CHECK_GL_ERROR();
}

void VAO::Bind() const {
#if defined(__APPLE__)
    glBindVertexArray(this->ID);
    // glBindVertexArrayAPPLE(ID);
#endif
#if defined(_WIN32) || defined(__linux__)
    glBindVertexArray(ID);
#endif
    CHECK_GL_ERROR();
}

void VAO::Unbind() {
#if defined(__APPLE__)
    glBindVertexArray(0);
    // glBindVertexArrayAPPLE(0);
#endif
#if defined(_WIN32) || defined(__linux__)
    glBindVertexArray(0);
#endif
    CHECK_GL_ERROR();
}

void VAO::Delete() const {
#if defined(__APPLE__)
    glDeleteVertexArrays(1, &this->ID);
    // glDeleteVertexArraysAPPLE(1, &ID);
#endif
#if defined(_WIN32) || defined(__linux__)
    glDeleteVertexArrays(1, &ID);
#endif
    CHECK_GL_ERROR();
}