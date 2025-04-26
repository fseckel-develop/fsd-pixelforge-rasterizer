#include "AmbientLight.h"


AmbientLight::AmbientLight(const vec3& color, const float intensity):
    Light(AMBIENT) {
    SetAmbient(color, intensity);
    this->diffuse = LightAttribute(vec3(0.0f), 0.0f);
    this->specular = LightAttribute(vec3(0.0f), 0.0f);
}
