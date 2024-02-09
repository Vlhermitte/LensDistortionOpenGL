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
vec4 pointLight() {

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
    float diff = max(dot(normal, lightDirection), 0.0);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;

    return (texture(diffuse0, texCoord) * (diffuse * attenuation + ambient) + texture(specular0, texCoord).r * specular * attenuation) * lightColor;
}

// directional light
vec4 directionalLight() {
    // lighting
    float ambient = 0.2f;

    // diffuse lighting
    vec3 normal = normalize(normal);
    vec3 lightDirection = normalize(vec3(0.0, 0.0, 1.0)); // constant light direction
    float diffuse = max(dot(normal, lightDirection), 0.0);

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;

    return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

// spot light
vec4 spotLight() {
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
    float theta = dot(lightDirection, normalize(-lightDirection));
    float epsilon = outerCutOff - innerCutOff;
    float intensity = clamp((theta - innerCutOff) / epsilon, 0.0, 1.0);

    return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
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
    vec4 lightResult = vec4(0.0, 0.0, 0.0, 1.0);
    if (usePointLight) {
        lightResult += pointLight();
    }
    if (useDirectionalLight) {
        lightResult += directionalLight();
    }
    if (useSpotLight) {
        lightResult += spotLight();
    }

    FragColor = lightResult;
}