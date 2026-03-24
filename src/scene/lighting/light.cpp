#include <pixelforge/scene/lighting/light.hpp>


namespace pixelforge::scene::lighting {

    using glm::vec3;
    using glm::clamp;


    Light::Light():
        type_(AMBIENT),
        ambient_(LightComponent(vec3(1.0f), 1.0f)),
        diffuse_(LightComponent(vec3(1.0f), 1.0f)),
        specular_(LightComponent(vec3(1.0f), 1.0f)) {
    }


    void Light::setColor(const vec3& color) {
        const vec3 clamped = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
        ambient_.color = clamped;
        diffuse_.color = clamped;
        specular_.color = clamped;
    }


    void Light::setIntensity(const float intensity) {
        const float clamped = clamp(intensity, 0.0f, 1.0f);
        ambient_.intensity = clamped;
        diffuse_.intensity = clamped;
        specular_.intensity = clamped;
    }


    void Light::setAmbient(const vec3& color, const float intensity) {
        const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
        const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
        ambient_ = LightComponent(clampedColor, clampedIntensity);
    }


    void Light::setDiffuse(const vec3& color, const float intensity) {
        const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
        const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
        diffuse_ = LightComponent(clampedColor, clampedIntensity);
    }


    void Light::setSpecular(const vec3& color, const float intensity) {
        const vec3 clampedColor = { clamp(color.r, 0.0f, 1.0f), clamp(color.g, 0.0f, 1.0f), clamp(color.b, 0.0f, 1.0f) };
        const float clampedIntensity = clamp(intensity, 0.0f, 1.0f);
        specular_ = LightComponent(clampedColor, clampedIntensity);
    }


    Light::Type Light::getType() const {
        return type_;
    }


    LightComponent Light::getAmbient() const {
        return ambient_;
    }


    LightComponent Light::getDiffuse() const {
        return diffuse_;
    }


    LightComponent Light::getSpecular() const {
        return specular_;
    }

} // namespace pixelforge::scene::lighting
