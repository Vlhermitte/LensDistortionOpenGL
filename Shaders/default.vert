#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 camMatrix; // view * projection

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 crntPos;

void main() {
    color = aColor;
    texCoord = aTexCoord;
    normal = aNormal;

    // current position
    crntPos = vec3(modelMatrix * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}