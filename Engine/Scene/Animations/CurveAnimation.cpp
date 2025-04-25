#include "CurveAnimation.h"
#include "../Transforms/Transform.h"
#include "../../Geometry/Curves/Curve.h"
#include <GLM/gtc/quaternion.hpp>


CurveAnimation::CurveAnimation(Curve* curve, const float duration, const Mode mode):
    Animation(duration, mode) {
    this->curve = curve;
}


Transform CurveAnimation::GetOffset() {
    Transform offset;
    const float t = GetProgress() * (curve->GetTMax() - curve->GetTMin()) + curve->GetTMin();
    offset.translation = curve->Evaluate(t);
    const mat3 frame = curve->ComputeRotMinFrame(t);
    offset.rotation = quat_cast(frame);
    return offset;
}
