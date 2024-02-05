//
// Created by Valentin on 04/02/2024.
//

#ifndef OPENGLPROJECT_DATA_H
#define OPENGLPROJECT_DATA_H

#include <GL/glew.h>
#include <string>


const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;


GLfloat squareVertices[] = {
        // position (x, y, z)                      color (r, g, b)               texCoord (x, y)
        -0.5f, -0.5f,0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left
        0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f,     1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f  // bottom right
};

GLuint squareIndices[] = {
        0, 2, 1,
        0, 3, 2
};

GLuint squareIndicesSize = sizeof(squareIndices) / sizeof(squareIndices[0]);


#endif //OPENGLPROJECT_DATA_H
