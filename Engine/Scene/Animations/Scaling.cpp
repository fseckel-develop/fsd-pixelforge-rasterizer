#include "Scaling.h"
#include "../Transforms/Transform.h"


Scaling::Scaling(const vec3 targetScale, const float duration, const Mode mode):
    Animation(duration, mode) {
    this->targetScale = targetScale;
}


Transform Scaling::GetOffset() {
    Transform offset;
    const float t = GetProgress();
    offset.scaling = mix(vec3(1.0f), targetScale, t);
    return offset;
}
