#include <pixelforge/scene/animation/curve_animation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <pixelforge/geometry/curves/curve.hpp>
#include <GLM/gtc/quaternion.hpp>


namespace pixelforge::scene::animation {

    using geometry::Curve;
    using transform::Transform;
    using std::shared_ptr;
    using glm::mat3;


    CurveAnimation::CurveAnimation(const Mode mode):
        Animation(mode) {
    }


    CurveAnimation::CurveAnimation(const shared_ptr<Curve>& curve, const float duration, const Mode mode):
        Animation(mode, duration) {
        this->curve_ = curve;
    }


    void CurveAnimation::setCurve(const shared_ptr<Curve>& curve) {
        this->curve_ = curve;
    }


    shared_ptr<Curve> CurveAnimation::getCurve() const {
        return curve_;
    }


    Transform CurveAnimation::getOffset() {
        Transform offset;
        const float t = getProgress() * (curve_->getTMax() - curve_->getTMin()) + curve_->getTMin();
        offset.setTranslation(curve_->evaluate(t));
        const mat3 frame = curve_->computeRotMinFrame(t);
        offset.setRotation(quat_cast(frame));
        return offset;
    }

} // namespace pixelforge::scene::animation
