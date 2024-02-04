#ifndef OPENGLPROJECT_VBO_H
#define OPENGLPROJECT_VBO_H

#include <GL/glew.h>
#include <GL/gl.h>



class VBO {
private:
    void check_error_gl() const;
public:
    GLuint ID;
    VBO(GLfloat* vertices, GLsizeiptr size);

    void Bind() const;
    static void Unbind();
    void Delete();
};


#endif //OPENGLPROJECT_VBO_H
