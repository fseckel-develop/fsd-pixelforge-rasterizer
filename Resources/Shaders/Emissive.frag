#version 410 core
#include "Shared/Light.glsl"

out vec4 fragmentColor;

uniform LightAttribute emissive;

void main() {
    fragmentColor = vec4(emissive.color * emissive.intensity, 1.0f);
}
