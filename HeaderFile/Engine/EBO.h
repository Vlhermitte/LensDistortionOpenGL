#ifndef OPENGLPROJECT_EBO_H
#define OPENGLPROJECT_EBO_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
#endif

#include "macro.h"

class EBO {
public:
    GLuint ID{};
    EBO(std::vector<GLuint>& indices);

    void Bind() const;
    void Unbind();
    void Delete();

};


#endif //OPENGLPROJECT_EBO_H
