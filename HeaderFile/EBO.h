#ifndef OPENGLPROJECT_EBO_H
#define OPENGLPROJECT_EBO_H

#include <GL/glew.h>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
#endif


class EBO {
private:
    void check_error_gl() const;
public:
    GLuint ID{};
    EBO(GLuint* indices, GLsizeiptr size);

    void Bind() const;
    void Unbind();
    void Delete();

};


#endif //OPENGLPROJECT_EBO_H
