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

uniform bool usePreProcessDistortion;
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

    // Apply a threshold to avoid instability with large distortions
    r = min(r, 1.5);

    // Calculate distortion factor with clamping for stability
    float distortionFactor = max(0.1, 1.0 + k1 * pow(r, 2) + k2 * pow(r, 4) + k3 * pow(r, 6));

    return distortionFactor * coord;
}

vec2 TangentialDistortion(vec2 coord, float p1, float p2) {
    float x = coord.x;
    float y = coord.y;
    float r2 = x * x + y * y;

    // Threshold radius to avoid instability
    r2 = min(r2, 2.0);

    // Calculate tangential distortion offsets
    float dx = 2.0 * p1 * x * y + p2 * (r2 + 2.0 * x * x);
    float dy = p1 * (r2 + 2.0 * y * y) + 2.0 * p2 * x * y;

    // Optional scaling to moderate distortion strength
    float scale = 1.0 / (1.0 + r2); // Falloff with radius
    dx *= scale;
    dy *= scale;

    // Clamp to avoid excessive distortion
    dx = clamp(dx, -1.0, 1.0);
    dy = clamp(dy, -1.0, 1.0);

    return vec2(dx, dy);
}

void main() {
    vec4 worldPosition = modelMatrix * vec4(aPos, 1.0); // Position in world coordinates
    vec4 clipPosition = camMatrix * worldPosition;      // Projected position (clip space)

    if (usePreProcessDistortion) {
        // Transform clip space coordinates to normalized device coordinates (NDC)
        vec3 ndc = clipPosition.xyz / clipPosition.w;  // Divide by w to get NDC (perspective division)

        // Apply radial and tangential distortion
        vec2 distortedCoord = RadialDistortion(ndc.xy, radialDistortionParams.x, radialDistortionParams.y, radialDistortionParams.z);
        distortedCoord += TangentialDistortion(ndc.xy, tangentialDistortionParams.x, tangentialDistortionParams.y);

        // Reconstruct distorted clip space position
        clipPosition.xy = distortedCoord * clipPosition.w;
    }

    position = vec3(worldPosition);                  // Output world position
    gl_Position = clipPosition;                     // Update the final vertex position

    color = aColor;                                 // Pass vertex color
    texCoord = aTexCoord;                           // Pass texture coordinate
    normal = normalize(vec3(normalMatrix * vec4(aNormal, 0.0))); // Transform and normalize normal vector

    // Calculate reflected vector
    vec3 viewVector = normalize(position - camPos); // View vector
    reflectedVector = reflect(viewVector, normal);  // Reflected vector

    // Transform to light space for shadow mapping
    fragPosLightSpace = lightSpaceMatrix * vec4(position, 1.0);
}