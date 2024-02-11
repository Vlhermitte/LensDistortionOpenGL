#ifndef OPENGLPROJECT_VBO_H
#define OPENGLPROJECT_VBO_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
#endif

typedef struct vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Color;
    glm::vec2 TexCoords;
} Vertex;

class VBO {
private:
    void check_error_gl() const;
public:
    GLuint ID;
    explicit VBO(std::vector<Vertex>& vertices);

    void Bind() const;
    void Unbind();
    void Delete();
};


#endif //OPENGLPROJECT_VBO_H
