#version 410 core

out vec4 FragColor;

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

// Matrices
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 camPos;

// Distortion and Texture
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 radialDistortionParams;
uniform vec2 tangentialDistortionParams;

// Lighting
uniform vec3 lightPos;
uniform vec4 lightColor;
uniform bool usePointLight;
uniform bool useDirectionalLight;
uniform bool useSpotLight;

struct Light {
    float diffuse;
    float specular;
};

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

// Point light
Light pointLight() {
    // distance from light to fragment
    float distance = length(lightPos - position);
    float a = 0.1f;
    float b = 0.064f;
    float attenuation = 1.0f / (1.0f + a * distance + b * pow(distance, 2)); // quadratic attenuation formula

    // lighting
    float ambient = 0.2f;

    // diffuse lighting
    vec3 normal = normalize(normal);
    vec3 lightDirection = normalize(lightPos - position);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;

    Light light = Light((diffuse * attenuation + ambient), specular * attenuation);
    return light;
}

// directional light
Light directionalLight() {
    // lighting
    float ambient = 0.2f;

    // diffuse lighting
    vec3 normal = normalize(normal);
    vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // constant light direction
    float diffuse = max(dot(normal, lightDirection), 0.0);

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;

    Light light = Light((diffuse + ambient), specular);
    return light;
}

// spot light
Light spotLight() {
    float outerCutOff = 0.90f;
    float innerCutOff = 0.95f;

    // lighting
    float ambient = 0.2f;

    // diffuse lighting
    vec3 normal = normalize(normal);
    vec3 lightDirection = normalize(lightPos - position);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;

    // spotlight
    float theta = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float intensity = clamp((theta - outerCutOff) / (innerCutOff - outerCutOff), 0.0, 1.0);

    Light light = Light((diffuse * intensity + ambient), specular * intensity);
    return light;
}

void main() {
    // Radial distortion
    vec2 uv = (texCoord * 2.0) - 1.0;
    vec2 rdv = RadialDistortion(uv, radialDistortionParams.x, radialDistortionParams.y, radialDistortionParams.z);
    rdv = (rdv + 1.0) / 2.0;
    // Tengential distortion (experimental)
    vec2 tdv = TangentialDistortion(uv, tangentialDistortionParams.x, tangentialDistortionParams.y);
    vec2 distortedCoord = rdv + tdv;

    // light calculations
    vec4 outputColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (usePointLight) {
        Light light = pointLight();
        outputColor += texture(diffuse0, distortedCoord) * light.diffuse;
        outputColor += texture(specular0, distortedCoord).r * light.specular;
        outputColor *= lightColor;
    }
    if (useDirectionalLight) {
        Light light = directionalLight();
        outputColor += texture(diffuse0, distortedCoord) * light.diffuse;
        outputColor += texture(specular0, distortedCoord).r * light.specular;
        outputColor *= lightColor;
    }
    if (useSpotLight) {
        Light light = spotLight();
        outputColor += texture(diffuse0, distortedCoord) * light.diffuse;
        outputColor += texture(specular0, distortedCoord).r * light.specular;
        outputColor *= lightColor;
    }

    FragColor = outputColor;
}