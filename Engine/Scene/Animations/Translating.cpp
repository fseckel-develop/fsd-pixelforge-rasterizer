#include "Translating.h"
#include "../Transforms/Transform.h"
#include "../../Utilities.h"


Translating::Translating(const Mode mode):
    Animation(mode),
    totalDistance(1.0f),
    direction(vec3(0.0f, 0.0f, 1.0f)) {
}


Translating::Translating(const Mode mode, const float duration, const vec3& direction, const float distance):
    Animation(mode, duration),
    direction() {
    this->totalDistance = distance;
    this->direction = Utilities::ValidateDirection(direction, "Translation::Translation");
}


void Translating::SetTotalDistance(const float distance) {
    this->totalDistance = distance;
}


void Translating::SetDirection(const vec3& direction) {
    this->direction = Utilities::ValidateDirection(direction, "Translation::SetDirection");
}


float Translating::GetTotalDistance() const {
    return totalDistance;
}


const vec3& Translating::GetDirection() const {
    return direction;
}


Transform Translating::GetOffset() {
    Transform offset;
    offset.SetTranslation(GetProgress() * totalDistance * direction);
    return offset;
}
