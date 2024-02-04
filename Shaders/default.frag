#version 410 core

out vec4 FragColor;

in vec3 color;
in vec3 normal;

void main() {
    vec3 lightDir = normalize(normal);
    FragColor = vec4(lightDir, 1.0);
    FragColor = vec4(color, 1.0);
}