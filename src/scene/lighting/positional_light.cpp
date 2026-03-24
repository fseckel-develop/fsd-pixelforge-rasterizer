#include <pixelforge/scene/lighting/positional_light.hpp>


namespace pixelforge::scene::lighting {

    using glm::vec3;


    PositionalLight::PositionalLight():
        position_(vec3(0.0f)),
        attenuation_(getAttenuationForRange(R100)) {
        this->type_ = POSITIONAL;
    }


    PositionalLight::PositionalLight(const vec3& position):
        position_(position),
        attenuation_(getAttenuationForRange(R100)) {
        this->type_ = POSITIONAL;
    }


    void PositionalLight::setPosition(const vec3& position) {
        this->position_= position;
    }


    void PositionalLight::setAttenuation(const Attenuation& attenuation) {
        this->attenuation_ = attenuation;
    }


    void PositionalLight::setRange(const Range range) {
        this->attenuation_ = getAttenuationForRange(range);
    }


    vec3 PositionalLight::getPosition() const {
        return position_;
    }


    Attenuation PositionalLight::getAttenuation() const {
        return this->attenuation_;
    }


    Attenuation PositionalLight::getAttenuationForRange(const Range range) {
        switch (range) {
            case R3250: return Attenuation(1.0, 0.0014, 0.000007);
            case R600: return Attenuation(1.0, 0.007, 0.0002);
            case R325: return Attenuation(1.0, 0.014, 0.0007);
            case R200: return Attenuation(1.0, 0.022, 0.0019);
            case R160: return Attenuation(1.0, 0.027, 0.0028);
            case R100: return Attenuation(1.0, 0.045, 0.0075);
            case R65: return Attenuation(1.0, 0.07, 0.017);
            case R50: return Attenuation(1.0, 0.09, 0.032);
            case R32: return Attenuation(1.0, 0.14, 0.07);
            case R20: return Attenuation(1.0, 0.22, 0.20);
            case R13: return Attenuation(1.0, 0.35, 0.44);
            case R7: return Attenuation(1.0, 0.7, 1.8);
            default: return Attenuation(1.0, 0.045, 0.0075);
        }
    }

} // namespace pixelforge::scene::lighting
