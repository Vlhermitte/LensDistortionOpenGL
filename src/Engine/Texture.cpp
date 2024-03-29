#include "Texture.h"


Texture::Texture(const char* image, const char* texType, GLenum slot) {
    type = texType;

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image, &width, &height, &nrChannels, 0);

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);
    CHECK_GL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format;
    GLenum internalFormat = GL_RGBA;
    switch (nrChannels) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            throw std::invalid_argument("Automatic Texture type recognition failed");
            break;
    }
    if (std::string(texType) == "normal") {
        format = GL_RGB;
        internalFormat = GL_RGB;
    }
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        CHECK_GL_ERROR();
    } else {
        std::cout << "Failed to load texture" << std::endl;
        glDeleteTextures(1, &ID);
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
    GLint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit + 2); // +2 because the texture class uses GL_TEXTURE2 as the first texture (GL_TEXTURE0 is reserved for the skybox texture and GL_TEXTURE1 shadow map texture)
    CHECK_GL_ERROR();
}

void Texture::Bind() {
    glActiveTexture(GL_TEXTURE2 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
    CHECK_GL_ERROR();
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
    CHECK_GL_ERROR();
}
