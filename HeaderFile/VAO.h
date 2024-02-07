//
// Created by Valentin Lhermitte on 02/02/2024.
//

#ifndef OPENGLPROJECT_VAO_H
#define OPENGLPROJECT_VAO_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "VBO.h"

class VAO {
private:
    void check_error_gl() const;
public:
    GLuint ID;
    VAO();

    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind() const;
    void Unbind() ;
    void Delete() const;


};


#endif //OPENGLPROJECT_VAO_H
