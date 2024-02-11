//
// Created by Valentin Lhermitte on 09/02/2024.
//

#ifndef OPENGLPROJECT_MODEL_H
#define OPENGLPROJECT_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

// Model class importing .obj 3D models using the Assimp library
class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    bool loadModel(std::string filename);
    static std::vector<Vertex> getVertices(const aiMesh *mesh);
    static std::vector<GLuint> getIndices(const aiMesh *mesh);
    static std::vector<Texture> getTextures(const aiMaterial *material, const std::string &filename);
public:
    explicit Model(const std::string &path);

    void setPosition(glm::vec3 position);
    void setScale(glm::vec3 scale);

    void Draw(Shader shader, Camera camera);
};


#endif //OPENGLPROJECT_MODEL_H
