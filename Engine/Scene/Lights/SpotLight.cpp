#include "SpotLight.h"


SpotLight::SpotLight(const vec3& position, const vec3& direction, const Attenuation& attenuation):
    PositionalLight(position, attenuation),
    direction(normalize(direction)),
    innerCutoff(cos(radians(5.0f))),
    outerCutoff(cos(radians(15.0f))) {
    this->type = SPOT;
}


SpotLight::SpotLight(const vec3& position, const vec3& direction, const LightRange range):
    PositionalLight(position, GetAttenuationForRange(range)),
    direction(normalize(direction)),
    innerCutoff(cos(radians(5.0f))),
    outerCutoff(cos(radians(15.0f))) {
    this->type = SPOT;
}


void SpotLight::SetDirection(const vec3& direction) {
    this->direction = normalize(direction);
}


void SpotLight::SetCutoffAngles(const float firstAngle, const float secondAngle) {
    if (firstAngle <= secondAngle) {
        this->innerCutoff = cos(radians(firstAngle));
        this->outerCutoff = cos(radians(secondAngle));
    }
    else {
        this->innerCutoff = cos(radians(secondAngle));
        this->outerCutoff = cos(radians(firstAngle));
    }
}


vec3 SpotLight::GetDirection() const {
    return direction;
}


float SpotLight::GetInnerCutoff() const {
    return this->innerCutoff;
}


float SpotLight::GetOuterCutoff() const {
    return this->outerCutoff;
}
