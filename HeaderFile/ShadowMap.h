//
// Created by Valentin Lhermitte on 17/02/2024.
//

#ifndef OPENGLPROJECT_SHADOWMAP_H
#define OPENGLPROJECT_SHADOWMAP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"

class ShadowMap {
private:
    GLuint depthMapFBO{};
    GLuint depthMap{};
    GLuint SHADOW_WIDTH;
    GLuint SHADOW_HEIGHT;
    glm::mat4 lightProjection{};
    glm::mat4 lightView{};
    glm::mat4 lightSpaceMatrix{};
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    void setupDepthMap();
public:
    ShadowMap();
    void Bind() const;
    static void Unbind();
    void Delete();
    void SetLightSpaceMatrix(glm::vec3 lightPos);
    glm::mat4 GetLightSpaceMatrix();
    GLuint GetDepthMap() const;
    void ActivateShadows(Shader &shadowMapShader, glm::vec3 lightPos);


};


#endif //OPENGLPROJECT_SHADOWMAP_H
