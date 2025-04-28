#include "DirectionalLight.h"


DirectionalLight::DirectionalLight():
    Light(),
    direction(vec3(0.0f, -1.0f, 0.0f)) {
    this->type = DIRECTIONAL;
}


DirectionalLight::DirectionalLight(const vec3& direction):
    Light(),
    direction(normalize(direction)) {
    this->type = DIRECTIONAL;
}


void DirectionalLight::SetDirection(const vec3& direction) {
    this->direction = normalize(direction);
}


vec3 DirectionalLight::GetDirection() const {
    return direction;
}
