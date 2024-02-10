//
// Created by Valentin Lhermitte on 09/02/2024.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh() {
    meshVAO.Bind();
    VBO meshVBO(vertices);
    EBO meshEBO(indices);
    meshVAO.LinkAttrib(meshVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // aPos
    meshVAO.LinkAttrib(meshVBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // aNormal
    meshVAO.LinkAttrib(meshVBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // aColor
    meshVAO.LinkAttrib(meshVBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float))); // aTexCoord
    meshVAO.Unbind();
    meshVBO.Unbind();
    meshEBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera) {
    shader.Activate();
    meshVAO.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string type = textures[i].type;
        if (type == "diffuse") {
            number = std::to_string(numDiffuse++);
        } else if (type == "specular") {
            number = std::to_string(numSpecular++);
        }
        textures[i].texUnit(shader, (type + number).c_str(), i);
        textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    meshVAO.Unbind();
}
