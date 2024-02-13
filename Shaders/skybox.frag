#version 410 core

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube texSkybox0;

void main() {
    FragColor = texture(texSkybox0, texCoords);
}