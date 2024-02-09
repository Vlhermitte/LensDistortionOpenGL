#include <iostream>
#include "VBO.h"

VBO::VBO(std::vector<Vertex>& vertices) {
    glGenBuffers(1, &this->ID);
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    check_error_gl();
}

void VBO::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
    check_error_gl();
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_error_gl();
}

void VBO::Delete() {
    glDeleteBuffers(1, &this->ID);
    check_error_gl();
}

void VBO::check_error_gl() const {
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
        std::cerr << "OpenGL error: " << error << " with VBO: (id=" << this->ID << ")" << std::endl;
    }
}