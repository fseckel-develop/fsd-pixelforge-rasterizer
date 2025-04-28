#include "PositionalLight.h"


PositionalLight::PositionalLight():
    position(vec3(0.0f)) {
    this->type = POSITIONAL;
}


PositionalLight::PositionalLight(const vec3& position):
    Light(),
    position(position) {
    this->type = POSITIONAL;
}


void PositionalLight::SetPosition(const vec3& position) {
    this->position= position;
}


void PositionalLight::SetAttenuation(const Attenuation& attenuation) {
    this->attenuation = attenuation;
}


void PositionalLight::SetRange(const LightRange range) {
    this->attenuation = GetAttenuationForRange(range);
}


vec3 PositionalLight::GetPosition() const {
    return position;
}


Attenuation PositionalLight::GetAttenuation() const {
    return this->attenuation;
}


Attenuation PositionalLight::GetAttenuationForRange(const LightRange range) {
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
