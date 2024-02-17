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
#include "macro.h"

typedef struct material {      // structure that describes currently used material
    glm::vec3  ambient;       // ambient component
    glm::vec3  diffuse;       // diffuse component
    glm::vec3  specular;      // specular component
    float shininess;     // sharpness of specular reflection
    bool  useTexture;    // defines whether the texture is used or not
} Material;

class Mesh {
private:
    void setupMesh();
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Material material;
    VAO meshVAO;
public:

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
    void setMaterial(Material material);

    void Draw(Shader& shader, Camera& camera, glm::mat4 modelMatrix);
    void Delete() const;
};


#endif //OPENGLPROJECT_MESH_H
