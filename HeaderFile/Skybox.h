//
// Created by Valentin Lhermitte on 13/02/2024.
//

#ifndef OPENGLPROJECT_SKYBOX_H
#define OPENGLPROJECT_SKYBOX_H

#include "Texture.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Skybox {
private:
    VAO skyboxVAO;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<std::string> faces;
    GLuint cubeMapTexture;
    void setupSkybox();
public:
    Skybox(std::vector<std::string>& faces, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
    void Draw(Shader& shader, Camera& camera);
    void Delete() const;
};


#endif //OPENGLPROJECT_SKYBOX_H
