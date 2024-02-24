//
// Created by Valentin Lhermitte on 09/02/2024.
//

#include "Model.h"

Model::Model(const std::string &path) {
    loadModel(path);
}

void Model::setPosition(glm::vec3 position) {
    this->position = position;
}

void Model::setScale(glm::vec3 scale) {
    this->scale = scale;
}

void Model::Draw(Shader shader, Camera camera) {
    for (auto & mesh : meshes) {
        // Apply transformations (translation, scale) to the model matrix
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, scale);
        mesh.Draw(shader, camera, modelMatrix);
    }
}

void Model::Delete() const {
    for (auto & mesh : meshes) {
        mesh.Delete();
    }
}

bool Model::loadModel(std::string filename) {
    Assimp::Importer importer;

    std::cout << "Loading model " << filename << std::endl;

    importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

    const aiScene* scene = importer.ReadFile(
            filename,
            0 |
            aiProcess_Triangulate |
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace |
            aiProcess_JoinIdenticalVertices |
            aiProcess_OptimizeMeshes |
            aiProcess_OptimizeGraph |
            aiProcess_SplitLargeMeshes
            );

    if (scene == nullptr) {
        std::cerr << "Assimp error loading model " << filename << std::endl;
        return false;
    }

    for (int i = 0; i < scene->mNumMeshes; i++) {
        std::cout << "Mesh " << i << " has " << scene->mMeshes[i]->mNumVertices << " vertices" << std::endl;
    }

    for (int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];

        std::vector<Vertex> vertices = getVertices(mesh);
        std::vector<GLuint> indices = getIndices(mesh);
        std::vector<Texture> textures = getTextures(scene->mMaterials[mesh->mMaterialIndex], filename);
        Material material = getMaterials(scene->mMaterials[mesh->mMaterialIndex]);

        meshes.emplace_back(vertices, indices, textures);
        meshes.back().setMaterial(material);
        // free memory
        vertices.clear();
        indices.clear();
        textures.clear();
    }
    return true;
}

std::vector<Vertex> Model::getVertices(const aiMesh *mesh) {
    std::vector<Vertex> vertices;

    for (int j = 0; j < mesh->mNumVertices; j++) {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[j].x;
        vector.y = mesh->mVertices[j].y;
        vector.z = mesh->mVertices[j].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[j].x;
        vector.y = mesh->mNormals[j].y;
        vector.z = mesh->mNormals[j].z;
        vertex.Normal = vector;

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][j].x;
            vec.y = mesh->mTextureCoords[0][j].y;
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        // Assuming models are not colored
        vertex.Color = glm::vec3(1.0f, 1.0f, 1.0f);
        vertices.push_back(vertex);
    }
    return vertices;
}

std::vector<GLuint> Model::getIndices(const aiMesh *mesh) {
    std::vector<GLuint> indices;
    for (int j = 0; j < mesh->mNumFaces; j++) {
        const aiFace& face = mesh->mFaces[j];
        for (int k = 0; k < face.mNumIndices; k++) {
            indices.push_back(face.mIndices[k]);
        }
    }
    return indices;
}

// For now, only diffuse and specular textures are supported
std::vector<Texture> Model::getTextures(const aiMaterial *material, const std::string &fileName) {
    std::vector<Texture> textures;
    if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
        for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, i, &path);
            std::string textureName = path.data;
            size_t found = fileName.find_last_of("/\\");
            if (found != std::string::npos) { // If found
                textureName.insert(0, fileName.substr(0, found + 1));
            }
            std::cout << "Loading diffuse file: " << textureName << std::endl;
            Texture texture(textureName.c_str(), "diffuse", 0);
            textures.push_back(texture);
        }
    } else {
        std::cout << "No diffuse texture" << std::endl;
    }
    if (material->GetTextureCount(aiTextureType_SPECULAR) != 0) {
        for (int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++) {
            aiString path;
            material->GetTexture(aiTextureType_SPECULAR, i, &path);
            std::string textureName = path.data;
            size_t found = fileName.find_last_of("/\\");
            if (found != std::string::npos) { // If found
                textureName.insert(0, fileName.substr(0, found + 1));
            }
            std::cout << "Loading specular file: " << textureName << std::endl;
            Texture texture(textureName.c_str(), "specular", 1);
            textures.push_back(texture);
        }
    } else {
        std::cout << "No specular texture" << std::endl;
    }
    if (material->GetTextureCount(aiTextureType_NORMALS) != 0) {
        for (int i = 0; i < material->GetTextureCount(aiTextureType_NORMALS); i++) {
            aiString path;
            material->GetTexture(aiTextureType_NORMALS, i, &path);
            std::string textureName = path.data;
            size_t found = fileName.find_last_of("/\\");
            if (found != std::string::npos) { // If found
                textureName.insert(0, fileName.substr(0, found + 1));
            }
            std::cout << "Loading normal file: " << textureName << std::endl;
            Texture texture(textureName.c_str(), "normal", 2);
            textures.push_back(texture);
        }
    } else {
        std::cout << "No normal texture" << std::endl;
    }
    return textures;
}

Material Model::getMaterials(const aiMaterial *material) {
    Material mat;
    aiColor3D color(0.f, 0.f, 0.f);
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color))
        mat.ambient = glm::vec3(color.r, color.g, color.b);
    else
        mat.ambient = glm::vec3(0.1f, 0.1f, 0.1f); // Default value
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
        mat.diffuse = glm::vec3(color.r, color.g, color.b);
    else
        mat.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Default value
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
        mat.specular = glm::vec3(color.r, color.g, color.b);
    else
        mat.specular = glm::vec3(0.5f, 0.5f, 0.5f); // Default value
    float shininess;
    if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess))
        mat.shininess = shininess;
    else
        mat.shininess = 32.0f; // Default value

    mat.useTexture = false;
    return mat;
}

glm::vec3 Model::getPosition() {
    return position;
}
