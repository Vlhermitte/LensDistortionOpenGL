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

uniform vec3 radialDistortionParams;
uniform vec2 tangentialDistortionParams;

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

vec2 TangentialDistortion(vec2 coord, float p1, float p2) {
    float x = coord.x;
    float y = coord.y;
    float r2 = x * x + y * y;
    float dx = 2.0 * p1 * x * y + p2 * (r2 + 2.0 * x * x);
    float dy = p1 * (r2 + 2.0 * y * y) + 2.0 * p2 * x * y;
    return vec2(dx, dy);
}

void main() {
    // Apply distortion on the vertex position
    // 1. Apply model matrix to get the world space position
    vec4 worldSpacePos = modelMatrix * vec4(aPos, 1.0);
    // 2. Apply view matrix to get the view space position
    vec4 viewSpacePos = viewMatrix * worldSpacePos;
    // 3. Apply projection matrix to get the clip space position
    vec4 clipSpacePos = projectionMatrix * viewSpacePos;
    // 4. Perspective division to get normalized device coordinates
    vec2 ndcPos = clipSpacePos.xy / clipSpacePos.w;
    // 5. Apply radial distortion to the normalized device coordinates
    vec2 radialDistortionCoords = RadialDistortion(ndcPos, radialDistortionParams.x, radialDistortionParams.y, radialDistortionParams.z);
    vec2 tangentialDistortionCoords = TangentialDistortion(ndcPos, tangentialDistortionParams.x, tangentialDistortionParams.y);
    vec2 distortedPos = radialDistortionCoords + tangentialDistortionCoords;
    // 6. Convert back to clip space
    vec4 clipSpaceDistortedPos = vec4(distortedPos * clipSpacePos.w, clipSpacePos.z, clipSpacePos.w);
    // 7. Convert back to view space
    vec4 viewSpaceDistortedPos = inverse(projectionMatrix) * clipSpaceDistortedPos;
    // 8. Convert back to world space
    vec4 worldSpaceDistortedPos = inverse(viewMatrix) * viewSpaceDistortedPos;

    color = aColor;
    texCoord = aTexCoord;
    normal = normalize(vec3(normalMatrix * vec4(aNormal, 0.0)));
    // current position
    position = worldSpaceDistortedPos.xyz;

    // reflect vector
    vec3 viewVector = normalize(position - camPos);
    reflectedVector = reflect(viewVector, normal);

    // light space matrix
    fragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);

    gl_Position = clipSpaceDistortedPos; // camMatrix * vec4(position, 1.0); // PVM * aPos
}