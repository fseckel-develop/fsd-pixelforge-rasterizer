#include "DirectionalLight.h"
#include "../../Utilities.h"
using namespace glm;


DirectionalLight::DirectionalLight():
    direction(vec3(0.0f, -1.0f, 0.0f)) {
    this->type = DIRECTIONAL;
}


DirectionalLight::DirectionalLight(const vec3& direction):
    direction(Utilities::ValidateDirection(direction, "DirectionalLight::DirectionalLight")) {
    this->type = DIRECTIONAL;
}


void DirectionalLight::SetDirection(const vec3& direction) {
    this->direction = Utilities::ValidateDirection(direction, "DirectionalLight::setDirection");
}


vec3 DirectionalLight::GetDirection() const {
    return direction;
}
