#version 410 core

in vec4 color;
in vec2 texCoord;

uniform sampler2D screenTexture;
uniform vec3 radialDistortionParams;
uniform vec2 tangentialDistortionParams;

out vec4 FragColor;

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
    vec2 coords = texCoord * 2.0 - 1.0;
    // Apply radial distortion
    vec2 radialDistortionCoords = RadialDistortion(coords, radialDistortionParams.x, radialDistortionParams.y, radialDistortionParams.z);
    // Apply tangential distortion
    vec2 tangentialDistortionCoords = TangentialDistortion(coords, tangentialDistortionParams.x, tangentialDistortionParams.y);

    // Combine the distortions
    vec2 distortedCoord = radialDistortionCoords + tangentialDistortionCoords;

    distortedCoord = (distortedCoord + 1.0) / 2.0;

    // Sample the texture with the distorted coordinates
    FragColor = color * texture(screenTexture, distortedCoord);
}