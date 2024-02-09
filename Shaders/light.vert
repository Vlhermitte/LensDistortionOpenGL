#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 camMatrix;

void main()
{
    gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0f);
}
