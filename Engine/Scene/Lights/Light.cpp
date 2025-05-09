#include "Light.h"
using namespace glm;


Light::Light():
    type(AMBIENT),
    ambient(LightComponent(vec3(1.0f), 1.0f)),
    diffuse(LightComponent(vec3(1.0f), 1.0f)),
    specular(LightComponent(vec3(1.0f), 1.0f)) {
}



void Light::SetColor(const vec3& color) {
    const vec3 clamped = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
    ambient.color = clamped;
    diffuse.color = clamped;
    specular.color = clamped;
}


void Light::SetIntensity(const float intensity) {
    const float clamped = clamp(intensity, 0.0f, 1.0f);
    ambient.intensity = clamped;
    diffuse.intensity = clamped;
    specular.intensity = clamped;
}


void Light::SetAmbient(const vec3& color, const float intensity) {
    const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
    ambient = LightComponent(clampedColor, clampedIntensity);
}


void Light::SetDiffuse(const vec3& color, const float intensity) {
    const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
    diffuse = LightComponent(clampedColor, clampedIntensity);
}


void Light::SetSpecular(const vec3& color, const float intensity) {
    const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
    specular = LightComponent(clampedColor, clampedIntensity);
}


Light::Type Light::GetType() const {
    return type;
}


LightComponent Light::GetAmbient() const {
    return ambient;
}


LightComponent Light::GetDiffuse() const {
    return diffuse;
}


LightComponent Light::GetSpecular() const {
    return specular;
}
