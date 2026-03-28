#version 410 core
#include "shared/light.glsl"

in vec2 textureCoordinate;
out vec4 fragmentColor;

uniform bool useEmissiveMap;
uniform sampler2D emissiveMap;
uniform LightAttribute emissive;

void main() {
    vec4 baseColor = useEmissiveMap ? texture(emissiveMap, textureCoordinate) : vec4(emissive.color, 1.0);
    fragmentColor = vec4(baseColor.rgb * emissive.intensity, baseColor.a);
}