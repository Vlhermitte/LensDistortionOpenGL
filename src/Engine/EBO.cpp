#include "EBO.h"
#include <string>
#include <iostream>

EBO::EBO(std::vector<GLuint>& indices) {
    glGenBuffers(1, &this->ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    CHECK_GL_ERROR();
}

void EBO::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
    CHECK_GL_ERROR();
}

void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR();
}

void EBO::Delete() {
    glDeleteBuffers(1, &this->ID);
    CHECK_GL_ERROR();
}
