//
// Created by Valentin Lhermitte on 13/02/2024.
//

#include "Skybox.h"
#include "macro.h"


Skybox::Skybox(std::vector<std::string>& faces, std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    this->faces = faces;
    this->vertices = vertices;
    this->indices = indices;
    setupSkybox();
}

void Skybox::setupSkybox() {
    glGenTextures(1, &cubeMapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR();

    skyboxVAO.Bind();
    VBO skyboxVBO(vertices);
    EBO skyboxEBO(indices);
    skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    skyboxVAO.Unbind();
    skyboxVBO.Unbind();
    skyboxEBO.Unbind();
    CHECK_GL_ERROR();
}

void Skybox::Draw(Shader& shader, Camera& camera) {
    glDepthFunc(GL_LEQUAL);
    shader.Activate();
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

    skyboxVAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    skyboxVAO.Unbind();
    shader.Deactivate();
    glDepthFunc(GL_LESS);
}