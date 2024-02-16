#version 410 core

out vec4 FragColor;

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texCoord;
in vec3 reflectedVector;

struct Light {
    float diffuse;
    float specular;
};

struct Material {      // structure that describes currently used material
    vec3  ambient;       // ambient component
    vec3  diffuse;       // diffuse component
    vec3  specular;      // specular component
    float shininess;     // sharpness of specular reflection
    bool  useTexture;    // defines whether the texture is used or not
};

// Matrices
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 camPos;

// Distortion and Texture
uniform samplerCube skybox;
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform vec3 radialDistortionParams;
uniform vec2 tangentialDistortionParams;

// Lighting
uniform vec3 lightPos;
uniform vec4 lightColor;
uniform bool usePointLight;
uniform bool useDirectionalLight;
uniform bool useSpotLight;
uniform Material material;


vec2 RadialDistortion(vec2 coord, float k1, float k2, float k3) {
    // NOT USED ANYMORE
    float r = length(coord);
    float distortionFactor = 1.0 + k1 * pow(r, 2) + k2 * pow(r, 4) + k3 * pow(r, 6);
    return distortionFactor * coord;
}

vec2 TangentialDistortion(vec2 coord, float p1, float p2) {
    // NOT USED ANYMORE
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
    // vec3 normalMap = 2.0 * texture(normal0, texCoord).xyz - 1.0;
    vec3 normal = normalize(normal);
    vec3 lightDirection = normalize(lightPos - position);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    vec3 halfwayVec = normalize(lightDirection + viewDir);
    float specular = pow(max(dot(normal, halfwayVec), 0.0), material.shininess);

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
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    vec3 halfwayVec = normalize(lightDirection + viewDir);
    float specular = pow(max(dot(normal, halfwayVec), 0.0), material.shininess);

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
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    vec3 halfwayVec = normalize(lightDirection + viewDir);
    float specular = pow(max(dot(normal, halfwayVec), 0.0), material.shininess);

    // spotlight
    float theta = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float intensity = clamp((theta - outerCutOff) / (innerCutOff - outerCutOff), 0.0, 1.0);

    Light light = Light((diffuse * intensity + ambient), specular * intensity);
    return light;
}

void main() {
    // light calculations
    vec4 outputColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (usePointLight) {
        Light light = pointLight();
        outputColor += texture(diffuse0, texCoord) * light.diffuse;
        outputColor += texture(specular0, texCoord).r * light.specular;
        outputColor *= lightColor;
    }
    if (useDirectionalLight) {
        Light light = directionalLight();
        outputColor += texture(diffuse0, texCoord) * light.diffuse;
        outputColor += texture(specular0, texCoord).r * light.specular;
        outputColor *= lightColor;
    }
    if (useSpotLight) {
        Light light = spotLight();
        outputColor += texture(diffuse0, texCoord) * light.diffuse;
        outputColor += texture(specular0, texCoord).r * light.specular;
        outputColor *= lightColor;
    }

    // skybox
    vec4 reflectedColor = texture(skybox, reflectedVector);
    outputColor = mix(outputColor, reflectedColor, 0.5f);

    FragColor = outputColor;
}