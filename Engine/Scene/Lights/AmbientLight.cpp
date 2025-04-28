#include "AmbientLight.h"


AmbientLight::AmbientLight():
    Light() {
    this->type = AMBIENT;
    this->diffuse = LightAttribute(vec3(0.0f), 0.0f);
    this->specular = LightAttribute(vec3(0.0f), 0.0f);
}


AmbientLight::AmbientLight(const vec3& color, const float intensity):
    Light() {
    this->type = AMBIENT;
    this->ambient = LightAttribute(color, intensity);
    this->diffuse = LightAttribute(vec3(0.0f), 0.0f);
    this->specular = LightAttribute(vec3(0.0f), 0.0f);
}
