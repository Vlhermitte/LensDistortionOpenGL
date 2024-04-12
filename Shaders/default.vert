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
uniform mat4 lightSpaceMatrix;

out vec3 position;
out vec3 color;
out vec3 normal;
out vec2 texCoord;
out vec3 reflectedVector;
out vec4 fragPosLightSpace;

vec2 RadialDistortion(vec2 coord, float k1, float k2, float k3) {
    float r = length(coord);
    float distortionFactor = 1.0 + k1 * pow(r, 2) + k2 * pow(r, 4) + k3 * pow(r, 6);
    return distortionFactor * coord;
}

void main() {
    // Apply distortion on the vertex position
    // 1. Apply projection matrix to the vertex position
    vec4 projectedPos = camMatrix * modelMatrix * vec4(aPos, 1.0);
    // 2. Go to clip space
    vec2 clipCoords = projectedPos.xy / projectedPos.w;
    // 3. Apply distortion
    vec2 distortion = RadialDistortion(clipCoords, 0.0, 0.1, 0.0);
    // 4. Go back to normalized device coordinates
    vec4 distortedPosClip = vec4(distortion * projectedPos.w, projectedPos.z, projectedPos.w);
    // 5. Go back to world space
    vec4 worldSpacePos = inverse(modelMatrix) * inverse(camMatrix) * distortedPosClip;

    color = aColor;
    texCoord = aTexCoord;
    normal = normalize(vec3(normalMatrix * vec4(aNormal, 0.0)));
    // current position
    position = vec3(modelMatrix * worldSpacePos);

    // reflect vector
    vec3 viewVector = normalize(position - camPos);
    reflectedVector = reflect(viewVector, normal);

    // light space matrix
    fragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);

    gl_Position = camMatrix * vec4(position, 1.0); // PVM * aPos
}