#include <pixelforge/scene/lighting/ambient_light.hpp>


namespace pixelforge::scene::lighting {

    using glm::vec3;


    AmbientLight::AmbientLight() {
        this->type_ = AMBIENT;
        this->diffuse_ = LightComponent(vec3(0.0f), 0.0f);
        this->specular_ = LightComponent(vec3(0.0f), 0.0f);
    }


    AmbientLight::AmbientLight(const vec3& color, const float intensity) {
        this->type_ = AMBIENT;
        this->ambient_ = LightComponent(color, intensity);
        this->diffuse_ = LightComponent(vec3(0.0f), 0.0f);
        this->specular_ = LightComponent(vec3(0.0f), 0.0f);
    }

} // namespace pixelforge::scene::lighting
