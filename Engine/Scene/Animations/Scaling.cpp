#include "Scaling.h"
#include "../Transforms/Transform.h"
using namespace glm;


Scaling::Scaling(const Mode mode):
    Animation(mode),
    targetScale(vec3(1.0f)) {
}


Scaling::Scaling(const Mode mode, const float duration, const vec3& targetScale):
    Animation(mode, duration),
    targetScale() {
    this->targetScale = targetScale;
}


void Scaling::SetTargetScale(const float scale) {
    this->targetScale = vec3(scale);
}


void Scaling::SetTargetScale(const vec3& scale) {
    this->targetScale = scale;
}


const vec3& Scaling::GetTargetScale() const {
    return targetScale;
}


Transform Scaling::GetOffset() {
    Transform offset;
    const float t = GetProgress();
    offset.SetScale(mix(vec3(1.0f), targetScale, t));
    return offset;
}
