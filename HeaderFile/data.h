//
// Created by Valentin on 04/02/2024.
//

#ifndef OPENGLPROJECT_DATA_H
#define OPENGLPROJECT_DATA_H

#include <GL/glew.h>
#include <string>



GLfloat cubeVertices[] = {
        // position (x, y, z)   u, v   normal (x, y, z)
        // Front face (z positive)
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

        // Back face (z negative)
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,   0.0f, 0.0f, -1.0f,

        // Left face (x negative)
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,

        // Right face (x positive)
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,

        // Top face (y positive)
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        // Bottom face (y negative)
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,   0.0f, -1.0f, 0.0f
};


GLuint cubeIndices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,

        // Back face
        4, 5, 6,
        6, 7, 4,

        // Left face
        8, 9, 10,
        10, 11, 8,

        // Right face
        12, 13, 14,
        14, 15, 12,

        // Top face
        16, 17, 18,
        18, 19, 16,

        // Bottom face
        20, 21, 22,
        22, 23, 20
};

GLuint cubeIndicesSize = sizeof(cubeIndices) / sizeof(cubeIndices[0]) / 3;

#endif //OPENGLPROJECT_DATA_H
