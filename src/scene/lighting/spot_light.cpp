#include <pixelforge/scene/lighting/spot_light.hpp>
#include <pixelforge/scene/utilities.hpp>


namespace pixelforge::scene::lighting {

    using glm::vec3;
    using glm::radians;


    SpotLight::SpotLight():
        direction_(vec3(0.0f, -1.0f, 0.0f)),
        innerCutoff_(cos(radians(5.0f))),
        outerCutoff_(cos(radians(15.0f))) {
        this->type_ = SPOT;
    }


    SpotLight::SpotLight(const vec3& position, const vec3& direction):
        PositionalLight(position),
        direction_(utilities::validateDirection(direction, "SpotLight::SpotLight")),
        innerCutoff_(cos(radians(5.0f))),
        outerCutoff_(cos(radians(15.0f))) {
        this->type_ = SPOT;
    }


    void SpotLight::setDirection(const vec3& direction) {
        this->direction_ = utilities::validateDirection(direction, "SpotLight::SetDirection");
    }


    void SpotLight::setCutoffAngles(const float innerCutoff, const float outerCutoff) {
        if (innerCutoff <= outerCutoff) {
            this->innerCutoff_ = cos(radians(innerCutoff));
            this->outerCutoff_ = cos(radians(outerCutoff));
        }
        else {
            this->innerCutoff_ = cos(radians(outerCutoff));
            this->outerCutoff_ = cos(radians(innerCutoff));
        }
    }


    vec3 SpotLight::getDirection() const {
        return direction_;
    }


    float SpotLight::getInnerCutoff() const {
        return this->innerCutoff_;
    }


    float SpotLight::getOuterCutoff() const {
        return this->outerCutoff_;
    }

} // namespace pixelforge::scene::lighting
