#version 410 core
#include "Shared/Light.glsl"
#include "Shared/Material.glsl"
const int MAX_LIGHT_COUNT = 10;

in vec3 fragmentPosition;
in vec2 textureCoordinate;
in vec3 fragmentNormal;

out vec4 fragmentColor;

uniform vec3 cameraPosition;
uniform Material material;
uniform int lightCount;
uniform Light lights[MAX_LIGHT_COUNT];


void main(void) {
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    Lighting lighting = NoLighting();
    for (int i = 0; i < lightCount; i++) {
        lighting = Add(lighting, EvaluateLight(lights[i], fragmentPosition, fragmentNormal, viewDirection, material.shininess));
    }
    vec4 materialDiffuse = material.useDiffuseMap ? vec4(texture(material.diffuseMap, textureCoordinate)) : material.diffuse;
    vec4 materialSpecular = material.useSpecularMap ? vec4(texture(material.specularMap, textureCoordinate)) : material.specular;
    vec4 ambientColor = vec4(lighting.ambient, 1.0f) * material.ambient;
    vec4 diffuseColor = vec4(lighting.diffuse, 1.0f) * materialDiffuse;
    vec4 specularColor = vec4(lighting.specular, 1.0f) * materialSpecular;
    fragmentColor = vec4((ambientColor + diffuseColor + specularColor).rgb, 1.0f);
}
