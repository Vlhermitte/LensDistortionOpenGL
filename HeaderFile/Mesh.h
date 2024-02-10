//
// Created by Valentin Lhermitte on 09/02/2024.
//

#ifndef OPENGLPROJECT_MESH_H
#define OPENGLPROJECT_MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"


class Mesh {
private:
    void setupMesh();
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VAO meshVAO;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
    void Draw(Shader& shader, Camera& camera, glm::mat4 modelMatrix);
};


#endif //OPENGLPROJECT_MESH_H
