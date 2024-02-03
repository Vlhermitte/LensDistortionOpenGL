#ifndef OPENGLPROJECT_EBO_H
#define OPENGLPROJECT_EBO_H

#include <OpenGL/gl3.h>

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
