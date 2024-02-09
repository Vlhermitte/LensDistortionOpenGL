//
// Created by Valentin on 05/02/2024.
//

#ifndef OPENGLPROJECT_TEXTURE_H
#define OPENGLPROJECT_TEXTURE_H

#include <GL/glew.h>
#include <stb/stb_image.h>
#include <string>

#include "Shader.h"


class Texture {
public:
    GLuint ID;
    const char* type;
    GLuint unit;

    Texture(const char* image, const char* texType, GLenum slot, GLenum format, GLenum pixelType);

    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();



};


#endif //OPENGLPROJECT_TEXTURE_H
