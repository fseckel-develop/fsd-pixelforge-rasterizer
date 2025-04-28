#include "Light.h"


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
    ambient = LightAttribute(clampedColor, clampedIntensity);
}


void Light::SetDiffuse(const vec3& color, const float intensity) {
    const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
    diffuse = LightAttribute(clampedColor, clampedIntensity);
}


void Light::SetSpecular(const vec3& color, const float intensity) {
    const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
    const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
    specular = LightAttribute(clampedColor, clampedIntensity);
}


LightType Light::GetType() const {
    return type;
}


LightAttribute Light::GetAmbient() const {
    return ambient;
}


LightAttribute Light::GetDiffuse() const {
    return diffuse;
}


LightAttribute Light::GetSpecular() const {
    return specular;
}
