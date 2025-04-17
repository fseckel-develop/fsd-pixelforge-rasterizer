#include "Translation.h"
#include "../Transforms//Transform.h"


Translation::Translation(const float distance, const vec3 direction, const float duration, const Mode mode):
    Animation(mode) {
    this->totalDistance = distance;
    if (length(direction) != 0.0f) {
        this->direction = normalize(direction);
    }
    this->duration = duration;
}


Transform Translation::GetOffset(const float deltaTime) {
    Transform offset;
    if (IsPlaying()) {
        elapsedTime += deltaTime;
        offset.translation = GetProgress() * totalDistance * direction;
    }
    return offset;
}
