#version 410 core

out vec4 FragColor;

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 texCoord;
in vec3 reflectedVector;
in vec4 fragPosLightSpace;

struct Light {
    float diffuse;
    float specular;
};

struct Material {      // structure that describes currently used material
    vec3  ambient;       // ambient component
    vec3  diffuse;       // diffuse component
    vec3  specular;      // specular component
    float shininess;     // sharpness of specular reflection
    float roughness;     // roughness of the material
    bool  useTexture;    // defines whether the texture is used or not
};

// Matrices
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 camPos;
uniform mat4 lightSpaceMatrix;

// Distortion and Texture
uniform samplerCube skybox;
uniform sampler2D shadowMap;
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
// uniform sampler2D roughness0; // Buggy  fix this

// Lighting
uniform vec3 lightPos;
uniform vec4 lightColor;
uniform bool usePointLight;
uniform bool useDirectionalLight;
uniform bool useSpotLight;
uniform Material material;


// Point light
Light pointLight() {
    // distance from light to fragment
    float distance = length(lightPos - position);
    float a = 0.1f;
    float b = 0.064f;
    float attenuation = 1.0f / (1.0f + a * distance + b * pow(distance, 2)); // quadratic attenuation formula

    // lighting
    float ambient = 0.05f;

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
    float ambient = 0.05f;

    // diffuse lighting
    vec3 normal = normalize(normal);
    vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // constant light direction
    float diffuse = max(dot(normal, lightDirection), 0.0);

    // specular lighting
    vec3 viewDir = normalize(camPos - position);
    vec3 reflectDir = reflect(-lightDirection, normal);
    vec3 halfwayVec = normalize(lightDirection + viewDir);
    float specular = pow(max(dot(normal, halfwayVec), 0.0), material.shininess);

    // Shadow
    float shadow = 0.0;
    vec4 lightCoords = fragPosLightSpace / fragPosLightSpace.w;
    if (lightCoords.z <= 1.0) {
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float closestDepth = texture(shadowMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;
        float bias = min(0.025 * (1.0 - dot(normal, lightDirection)), 0.005);
        // Soft shadows
        int sampleRadius = 4;
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for (int x = -sampleRadius; x <= sampleRadius; ++x) {
            for (int y = -sampleRadius; y <= sampleRadius; ++y) {
                float pcfDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= pow(2 * sampleRadius + 1, 2);
    }

    Light light = Light((diffuse + ambient) * (1.0 - shadow), specular * (1.0 - shadow));
    return light;
}

// spot light
Light spotLight() {
    float outerCutOff = 0.90f;
    float innerCutOff = 0.95f;

    // lighting
    float ambient = 0.05f;

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
    vec4 outputColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Sample textures
    vec4 texColor = texture(diffuse0, texCoord);
    vec4 specularColor = texture(specular0, texCoord);

    if (usePointLight) {
        Light light = pointLight();
        outputColor += texColor * vec4(material.diffuse + material.ambient, 1.0f) * light.diffuse;
        outputColor += specularColor * vec4(material.specular, 1.0f) * light.specular;
        outputColor *= lightColor;
    }
    if (useDirectionalLight) {
        Light light = directionalLight();
        outputColor += texColor * vec4(material.diffuse + material.ambient, 1.0f) * light.diffuse;
        outputColor += specularColor * vec4(material.specular, 1.0f) * light.specular;
        outputColor *= lightColor;
    }
    if (useSpotLight) {
        Light light = spotLight();
        outputColor += texColor * vec4(material.diffuse + material.ambient, 1.0f) * light.diffuse;
        outputColor += specularColor * vec4(material.specular, 1.0f) * light.specular;
        outputColor *= lightColor;
    }

    // skybox
    vec4 reflectedColor = texture(skybox, reflectedVector);
    // mix skybox and object color depending on the material roughness
    // float roughness = texture(roughness0, texCoord).r; // Buggy fix this
    outputColor = mix(outputColor, reflectedColor, material.roughness);

    FragColor = outputColor;
}