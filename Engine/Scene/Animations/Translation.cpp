#include "Translation.h"
#include "../Transforms//Transform.h"


Translation::Translation(const float distance, const vec3 direction, const float duration, const Mode mode):
    Animation(duration, mode) {
    this->totalDistance = distance;
    if (length(direction) != 0.0f) {
        this->direction = normalize(direction);
    }
}


float Translation::GetTotalDistance() const {
    return totalDistance;
}


const vec3& Translation::GetDirection() const {
    return direction;
}


Transform Translation::GetOffset() {
    Transform offset;
    offset.translation = GetProgress() * totalDistance * direction;
    return offset;
}
