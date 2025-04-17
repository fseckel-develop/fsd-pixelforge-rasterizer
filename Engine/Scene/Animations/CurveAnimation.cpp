#include "CurveAnimation.h"
#include "../Transforms/Transform.h"
#include "../../Geometry/Curves/Curve.h"
#include <GLM/gtc/quaternion.hpp>


CurveAnimation::CurveAnimation(Curve* curve, const float duration, const Mode mode):
    Animation(mode) {
    this->curve = curve;
    this->duration = duration;
}


Transform CurveAnimation::GetOffset(const float deltaTime) {
    Transform offset;
    if (IsPlaying()) {
        elapsedTime += deltaTime;
        const float t = GetProgress() * (curve->GetTMax() - curve->GetTMin()) + curve->GetTMin();
        offset.translation = curve->Evaluate(t);
        const mat3 frame = curve->ComputeRotMinFrame(t);
        offset.rotation = quat_cast(frame);
    }
    return offset;
}
