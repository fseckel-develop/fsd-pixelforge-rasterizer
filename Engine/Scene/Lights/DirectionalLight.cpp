#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(const string& name, const vec3& direction):
    Light(name, DIRECTIONAL),
    direction(normalize(direction)) {
}


void DirectionalLight::SetDirection(const vec3& direction) {
    this->direction = normalize(direction);
}


vec3 DirectionalLight::GetCurrentDirection() const {
    return normalize(vec3(GetModelMatrix() * vec4(direction, 0.0f)));
}
