//
// Created by Valentin Lhermitte on 02/02/2024.
//

#ifndef OPENGLPROJECT_VAO_H
#define OPENGLPROJECT_VAO_H

#include <GL/glew.h>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
#endif
#include "VBO.h"
#include "macro.h"

class VAO {
public:
    GLuint ID;
    VAO();

    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind() const;
    void Unbind() ;
    void Delete() const;


};


#endif //OPENGLPROJECT_VAO_H
