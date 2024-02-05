#version 410 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform vec3 radialDistortionParams;
uniform vec2 tangentialDistortionParams;

vec2 RadialDistortion(vec2 coord, float k1, float k2, float k3) {
    float r = length(coord);
    float distortionFactor = 1.0 + k1 * pow(r, 2) + k2 * pow(r, 4) + k3 * pow(r, 6);
    return distortionFactor * coord;
}

vec2 TangentialDistortion(vec2 coord, float p1, float p2) {
    // EXPERIMENTAL - MIGHT BE WRONG
    float x = coord.x;
    float y = coord.y;
    float r2 = x * x + y * y;
    float dx = 2.0 * p1 * x * y + p2 * (r2 + 2.0 * x * x);
    float dy = p1 * (r2 + 2.0 * y * y) + 2.0 * p2 * x * y;
    return vec2(dx, dy);
}

void main() {

    // Radial distortion
    vec2 uv = (texCoord * 2.0) - 1.0;
    vec2 rdv = RadialDistortion(uv, radialDistortionParams.x, radialDistortionParams.y, radialDistortionParams.z);
    rdv = (rdv + 1.0) / 2.0;
    //rdv = clamp(dv, 0.0, 1.0);

    // Tengential distortion (experimental)
    vec2 tdv = TangentialDistortion(uv, tangentialDistortionParams.x, tangentialDistortionParams.y);

    vec2 distortedCoord = rdv + tdv;

    FragColor = texture(tex0, distortedCoord);
}