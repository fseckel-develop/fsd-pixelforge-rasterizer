#include "AmbientLight.h"


AmbientLight::AmbientLight() {
    this->type = AMBIENT;
    this->diffuse = LightAttribute(vec3(0.0f), 0.0f);
    this->specular = LightAttribute(vec3(0.0f), 0.0f);
}


AmbientLight::AmbientLight(const vec3& color, const float intensity) {
    this->type = AMBIENT;
    this->ambient = LightAttribute(color, intensity);
    this->diffuse = LightAttribute(vec3(0.0f), 0.0f);
    this->specular = LightAttribute(vec3(0.0f), 0.0f);
}
