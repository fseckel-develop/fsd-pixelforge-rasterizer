#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/utilities.hpp>


namespace pixelforge::scene::lighting {

    using glm::vec3;


    DirectionalLight::DirectionalLight():
        direction_(vec3(0.0f, -1.0f, 0.0f)) {
        this->type_ = DIRECTIONAL;
    }


    DirectionalLight::DirectionalLight(const vec3& direction):
        direction_(Utilities::validateDirection(direction, "DirectionalLight::DirectionalLight")) {
        this->type_ = DIRECTIONAL;
    }


    void DirectionalLight::setDirection(const vec3& direction) {
        this->direction_ = Utilities::validateDirection(direction, "DirectionalLight::setDirection");
    }


    vec3 DirectionalLight::getDirection() const {
        return direction_;
    }

} // namespace pixelforge::scene::lighting
