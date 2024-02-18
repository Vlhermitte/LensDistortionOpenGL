//
// Created by Valentin Lhermitte on 17/02/2024.
//

#include "ShadowMap.h"
#include "macro.h"


ShadowMap::ShadowMap() {
    SHADOW_WIDTH = 2048;
    SHADOW_HEIGHT = 2048;
    setupDepthMap();
}

void ShadowMap::setupDepthMap() {
    // Create depth map FBO
    glGenFramebuffers(1, &depthMapFBO);
    CHECK_GL_ERROR();

    // Create depth map texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    CHECK_GL_ERROR();

    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    CHECK_GL_ERROR();

    // Attach depth map as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CHECK_GL_ERROR();
}

void ShadowMap::Bind() const {
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR();
}

void ShadowMap::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CHECK_GL_ERROR();
}

void ShadowMap::Delete() {
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
    CHECK_GL_ERROR();
}

void ShadowMap::SetLightSpaceMatrix(glm::vec3 lightPos) {
    glm::mat4 orthoProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = orthoProjection * lightView;
}

glm::mat4 ShadowMap::GetLightSpaceMatrix() {
    return lightSpaceMatrix;
}

GLuint ShadowMap::GetDepthMap() const {
    return depthMap;
}

void ShadowMap::ActivateShadows(Shader &shadowMapShader, glm::vec3 lightPos) {
    SetLightSpaceMatrix(lightPos);
    shadowMapShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(GetLightSpaceMatrix()));
    CHECK_GL_ERROR();
    Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
}