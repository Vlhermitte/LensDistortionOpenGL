//
// Created by Valentin on 04/02/2024.
//

#ifndef OPENGLPROJECT_DATA_H
#define OPENGLPROJECT_DATA_H

#include "VBO.h"

#include <GL/glew.h>
#include <string>

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Skybox vertices
Vertex skyboxVertices[] = {
        Vertex{glm::vec3(-1.0f,  -1.0f, 1.0f)},
        Vertex{glm::vec3( 1.0f, -1.0f, 1.0f)},
        Vertex{glm::vec3( 1.0f, -1.0f, -1.0f)},
        Vertex{glm::vec3(-1.0f, -1.0f, -1.0f)},
        Vertex{glm::vec3(-1.0f,  1.0f,  1.0f)},
        Vertex{glm::vec3( 1.0f,  1.0f,  1.0f)},
        Vertex{glm::vec3( 1.0f,  1.0f, -1.0f)},
        Vertex{glm::vec3(-1.0f,  1.0f, -1.0f)}
};

// Skybox indices
GLuint skyboxIndices[] = {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4,
        4, 5, 1, 1, 0, 4,
        3, 2, 6, 6, 7, 3
};


#endif //OPENGLPROJECT_DATA_H
