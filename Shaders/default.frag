#version 410 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform vec3 barrelDistortionParams;

vec2 BarrelDistortion(vec2 coord, float k1, float k2, float k3) {
    float r = length(coord);
    float r2 = r * r;
    float r4 = r2 * r2;
    float r6 = r4 * r2;
    float r8 = r4 * r4;
    float r_d = 1.0 + k1 * r2 + k2 * r4 + k3 * r6;
    return coord * r_d;
}

vec2 TengentialDistortion(vec2 coord, float p1, float p2) {
    float x = coord.x * (1.0 + p1 * (coord.x * coord.x + coord.y * coord.y));
    float y = coord.y * (1.0 + p2 * (coord.x * coord.x + coord.y * coord.y));
    return vec2(x, y);
}

void main() {

    vec2 uv = (gl_FragCoord.xy / 640.0f) * 2.0 - 1.0;
    vec2 dv = BarrelDistortion(uv, barrelDistortionParams.x, barrelDistortionParams.y, barrelDistortionParams.z);

    // FragColor = vec4(color, 1.0);
    FragColor = texture(tex0, texCoord + dv);
}