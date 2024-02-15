#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 texCoords;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main() {
    vec3 worldView = normalize(cameraPos - aPos);
    vec3 reflectDir = reflect(-worldView, normalize(aNormal));
    float m = 2.0 * sqrt(reflectDir.x * reflectDir.x +
                         reflectDir.y * reflectDir.y +
                        (reflectDir.z + 1.0) * (reflectDir.z + 1.0)
    );
    texCoords = reflectDir / m + 0.5;

    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}