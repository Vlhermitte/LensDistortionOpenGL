//
// Created by Valentin Lhermitte on 19/02/2024.
//

#include "Framebuffer.h"

// Quad vertices (for custom framebuffer)
std::vector<Vertex> quadVertices = {
        // positions (x,y)       // texture Coords (s,t)
        Vertex{glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}
};

// Quad indices (for custom framebuffer)
std::vector<GLuint> quadIndices = {
        0, 1, 2,
        2, 3, 0
};

Framebuffer::Framebuffer(int width, int height) {
    this->width = width;
    this->height = height;

    // Create framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    CHECK_GL_ERROR();

    // Create texture
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR();

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
    CHECK_GL_ERROR();

    // Create renderbuffer object for depth and stencil attachment
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    CHECK_GL_ERROR();

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create VAO for the framebuffer
    framebufferVAO.Bind();
    VBO vbo(quadVertices);
    EBO ebo(quadIndices);

    framebufferVAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    framebufferVAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    framebufferVAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    framebufferVAO.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    framebufferVAO.Unbind();
    vbo.Unbind();
    ebo.Unbind();
    CHECK_GL_ERROR();
}

void Framebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    CHECK_GL_ERROR();
}

void Framebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    CHECK_GL_ERROR();
}

void Framebuffer::Delete() const {
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &framebufferTexture);
    CHECK_GL_ERROR();
}

void Framebuffer::Draw(Shader shader) {
    // Making sure we have the correct framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shader.Activate();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glUniform1i(glGetUniformLocation(shader.ID, "screenTexture"), 0);
    CHECK_GL_ERROR();

    framebufferVAO.Bind();
    glDisable(GL_DEPTH_TEST);
    CHECK_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();

    shader.Deactivate();
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();
}