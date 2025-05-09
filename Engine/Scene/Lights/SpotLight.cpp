#include "SpotLight.h"
#include "../../Utilities.h"
using namespace glm;


SpotLight::SpotLight():
    direction(vec3(0.0f, -1.0f, 0.0f)),
    innerCutoff(cos(radians(5.0f))),
    outerCutoff(cos(radians(15.0f))) {
    this->type = SPOT;
}


SpotLight::SpotLight(const vec3& position, const vec3& direction):
    PositionalLight(position),
    direction(Utilities::ValidateDirection(direction, "SpotLight::SpotLight")),
    innerCutoff(cos(radians(5.0f))),
    outerCutoff(cos(radians(15.0f))) {
    this->type = SPOT;
}


void SpotLight::SetDirection(const vec3& direction) {
    this->direction = Utilities::ValidateDirection(direction, "SpotLight::SetDirection");
}


void SpotLight::SetCutoffAngles(const float innerCutoff, const float outerCutoff) {
    if (innerCutoff <= outerCutoff) {
        this->innerCutoff = cos(radians(innerCutoff));
        this->outerCutoff = cos(radians(outerCutoff));
    }
    else {
        this->innerCutoff = cos(radians(outerCutoff));
        this->outerCutoff = cos(radians(innerCutoff));
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
