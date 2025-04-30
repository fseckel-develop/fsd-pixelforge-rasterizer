#include "CurveAnimation.h"
#include "../Transforms/Transform.h"
#include "../../Geometry/Curves/Curve.h"
#include <GLM/gtc/quaternion.hpp>


CurveAnimation::CurveAnimation(const Mode mode):
    Animation(mode) {
}


CurveAnimation::CurveAnimation(const shared_ptr<Curve>& curve, const float duration, const Mode mode):
    Animation(mode, duration) {
    this->curve = curve;
}


void CurveAnimation::SetCurve(const shared_ptr<Curve>& curve) {
    this->curve = curve;
}


shared_ptr<Curve> CurveAnimation::GetCurve() const {
    return curve;
}


Transform CurveAnimation::GetOffset() {
    Transform offset;
    const float t = GetProgress() * (curve->GetTMax() - curve->GetTMin()) + curve->GetTMin();
    offset.SetTranslation(curve->Evaluate(t));
    const mat3 frame = curve->ComputeRotMinFrame(t);
    offset.SetRotation(quat_cast(frame));
    return offset;
}
