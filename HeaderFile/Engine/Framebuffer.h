//
// Created by Valentin Lhermitte on 19/02/2024.
//

#ifndef OPENGLPROJECT_FRAMEBUFFER_H
#define OPENGLPROJECT_FRAMEBUFFER_H

#include <iostream>
#include <GL/glew.h>

#include "macro.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

class Framebuffer {
private:
    unsigned int framebuffer;
    unsigned int framebufferTexture;
    int width;
    int height;
    VAO framebufferVAO;
public:
    Framebuffer(int width, int height);
    void Bind() const;
    void Unbind() const;
    void Delete() const;
    void Draw(Shader shader);
};


#endif //OPENGLPROJECT_FRAMEBUFFER_H
