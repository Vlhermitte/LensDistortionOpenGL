#include <iostream>
#include "VBO.h"

VBO::VBO(std::vector<Vertex>& vertices) {
    glGenBuffers(1, &this->ID);
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    CHECK_GL_ERROR();
}

void VBO::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
    CHECK_GL_ERROR();
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR();
}

void VBO::Delete() {
    glDeleteBuffers(1, &this->ID);
    CHECK_GL_ERROR();
}