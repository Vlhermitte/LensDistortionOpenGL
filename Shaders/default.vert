#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform mat4 camMatrix; // view * projection
uniform vec3 camPos;

out vec3 position;
out vec3 color;
out vec3 normal;
out vec2 texCoord;
out vec3 reflectedVector;

void main() {
    color = aColor;
    texCoord = aTexCoord;
    normal = normalize(vec3(normalMatrix * vec4(aNormal, 0.0)));
    // current position
    position = vec3(modelMatrix * vec4(aPos, 1.0f));

    // reflect vector
    vec3 viewVector = normalize(position - camPos);
    reflectedVector = reflect(viewVector, normal);

    gl_Position = camMatrix * vec4(position, 1.0); // PVM * aPos
}