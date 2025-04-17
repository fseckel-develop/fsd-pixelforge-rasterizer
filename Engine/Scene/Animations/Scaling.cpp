#include "Scaling.h"
#include "../Transforms/Transform.h"


Scaling::Scaling(const vec3 targetScale, const float duration, const Mode mode):
    Animation(mode) {
    this->targetScale = targetScale;
    this->duration = duration;
}


Transform Scaling::GetOffset(const float deltaTime) {
    Transform offset;
    if (IsPlaying()) {
        elapsedTime += deltaTime;
        const float t = GetProgress();
        offset.scaling = mix(vec3(1.0f), targetScale, t);
    }
    return offset;
}
