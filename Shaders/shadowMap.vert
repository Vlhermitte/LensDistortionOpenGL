#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

void main() {
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(aPos, 1.0);
}
