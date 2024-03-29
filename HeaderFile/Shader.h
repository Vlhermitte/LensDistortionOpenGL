//
// Created by Valentin Lhermitte on 02/02/2024.
//

#ifndef OPENGLPROJECT_SHADER_H
#define OPENGLPROJECT_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "macro.h"


class Shader {
public:
    GLint ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void Activate() const;
    void Deactivate() const;
    void Delete() const;
};


#endif //OPENGLPROJECT_SHADER_H
