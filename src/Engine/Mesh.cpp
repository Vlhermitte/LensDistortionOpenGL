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
    CHECK_GL_ERROR();
    VBO meshVBO(vertices);
    EBO meshEBO(indices);
    meshVAO.LinkAttrib(meshVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // aPos
    meshVAO.LinkAttrib(meshVBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // aNormal
    meshVAO.LinkAttrib(meshVBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // aColor
    meshVAO.LinkAttrib(meshVBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float))); // aTexCoord
    meshVAO.Unbind();
    meshVBO.Unbind();
    meshEBO.Unbind();
    CHECK_GL_ERROR();
}

void Mesh::setMaterial(Material material) {
    this->material = material;
}

void Mesh::Draw(Shader& shader, Camera& camera, glm::mat4 modelMatrix) {
    shader.Activate();
    meshVAO.Bind();
    CHECK_GL_ERROR();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;
    unsigned int numNormal = 0;
    unsigned int numRoughness = 0;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string type = textures[i].type;
        if (type == "diffuse") {
            number = std::to_string(numDiffuse++);
        } else if (type == "specular") {
            number = std::to_string(numSpecular++);
        } else if (type == "normal") {
            number = std::to_string(numNormal++);
        } else if (type == "roughness" || type == "metallic" || type == "ao" || type == "reflection") {
            number = std::to_string(numRoughness++);
        }
        textures[i].texUnit(shader, (type + number).c_str(), i);
        textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "material.ambient"), material.ambient.x, material.ambient.y, material.ambient.z);
    glUniform3f(glGetUniformLocation(shader.ID, "material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
    glUniform3f(glGetUniformLocation(shader.ID, "material.specular"), material.specular.x, material.specular.y, material.specular.z);
    glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), material.shininess);
    glUniform1f(glGetUniformLocation(shader.ID, "material.roughness"), material.roughness);
    CHECK_GL_ERROR();

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");
    CHECK_GL_ERROR();

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    CHECK_GL_ERROR();
    const glm::mat3 modelRotationMatrix = glm::mat3(
            modelMatrix[0],
            modelMatrix[1],
            modelMatrix[2]
    );
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat4(glm::mat3(modelRotationMatrix))));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    CHECK_GL_ERROR();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();
    meshVAO.Unbind();
    shader.Deactivate();
}

void Mesh::Delete() const {
    meshVAO.Delete();
}