//
// Created by Valentin on 04/02/2024.
//

#ifndef OPENGLPROJECT_DATA_H
#define OPENGLPROJECT_DATA_H

#include <GL/glew.h>
#include <string>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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

// Cube vertices
GLfloat cubeVertices[] = {
        // position (x, y, z)                      normal (x, y ,z)              texCoord (x, y)
        // Front face (z positive)
        // Front face (z positive)
        -0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // Back face (z negative)
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

        // Left face (x negative)
        -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // Right face (x positive)
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        // Top face (y positive)
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        // Bottom face (y negative)
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

GLuint cubeIndices[] = {
        // Front face
        0, 1, 2,
        0, 2, 3,

        // Back face
        4, 5, 6,
        4, 6, 7,

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

GLfloat pyramidVertices[] = { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

GLuint pyramidIndices[] = {
    0, 1, 2, // Bottom side
    0, 2, 3, // Bottom side
    4, 6, 5, // Left side
    7, 9, 8, // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14 // Facing side
};

// Sun vertices (Cube)

GLfloat sunVertices[] = {
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
        0.1f,  0.1f, -0.1f,
        0.1f,  0.1f,  0.1f
};

GLuint sunIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
};


#endif //OPENGLPROJECT_DATA_H
