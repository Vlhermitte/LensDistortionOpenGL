#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec4 color;
out vec2 texCoord;

void main() {
    color = vec4(aColor, 1.0);
    texCoord = aTexCoord;
    gl_Position = vec4(aPos, 1.0);
}