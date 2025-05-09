#include "AmbientLight.h"
using namespace glm;


AmbientLight::AmbientLight() {
    this->type = AMBIENT;
    this->diffuse = LightComponent(vec3(0.0f), 0.0f);
    this->specular = LightComponent(vec3(0.0f), 0.0f);
}


AmbientLight::AmbientLight(const vec3& color, const float intensity) {
    this->type = AMBIENT;
    this->ambient = LightComponent(color, intensity);
    this->diffuse = LightComponent(vec3(0.0f), 0.0f);
    this->specular = LightComponent(vec3(0.0f), 0.0f);
}
