#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(const vec3& direction):
    Light(DIRECTIONAL),
    direction(normalize(direction)) {
}


void DirectionalLight::SetDirection(const vec3& direction) {
    this->direction = normalize(direction);
}


vec3 DirectionalLight::GetDirection() const {
    return direction;
}
