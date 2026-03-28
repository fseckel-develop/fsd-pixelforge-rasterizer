#include "builders/compiler/animation_compiler.hpp"
#include <pixelforge/scene/animation/curve_animation.hpp>
#include <pixelforge/scene/animation/elliptic_orbiting.hpp>
#include <pixelforge/scene/animation/keyframe_animation.hpp>
#include <pixelforge/scene/animation/orbiting.hpp>
#include <pixelforge/scene/animation/rotation.hpp>
#include <pixelforge/scene/animation/scaling.hpp>
#include <pixelforge/scene/animation/translation.hpp>
#include <variant>


namespace pixelforge::scene::builders::compiler {

    template<class>
    inline constexpr bool always_false_v = false;

    std::shared_ptr<animation::Animation> buildAnimation(
        const pixelforge::builders::specification::AnimationSpec& spec)
    {
        return std::visit([]<typename dataT>(const dataT& data) -> std::shared_ptr<animation::Animation> {
            using T = std::decay_t<dataT>;

            if constexpr (std::is_same_v<T, pixelforge::builders::specification::RotationAnimationSpec>) {
                auto animation = std::make_shared<animation::Rotation>(data.mode);
                if (data.duration) animation->setDuration(*data.duration);
                if (data.rotationAxis) animation->setRotationAxis(*data.rotationAxis);
                if (data.totalAngle) animation->setTotalAngle(*data.totalAngle);
                return animation;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::OrbitingAnimationSpec>) {
                auto animation = std::make_shared<animation::Orbiting>(data.mode);
                if (data.duration) animation->setDuration(*data.duration);
                if (data.radius) animation->setRadius(*data.radius);
                if (data.rotationAxis) animation->setRotationAxis(*data.rotationAxis);
                if (data.totalAngle) animation->setTotalAngle(*data.totalAngle);
                return animation;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::EllipticOrbitingAnimationSpec>) {
                auto animation = std::make_shared<animation::EllipticOrbiting>(data.mode);
                if (data.duration) animation->setDuration(*data.duration);
                if (data.semiMajorAxis) animation->setSemiMajorAxis(*data.semiMajorAxis);
                if (data.semiMinorAxis) animation->setSemiMinorAxis(*data.semiMinorAxis);
                if (data.rotationAxis) animation->setRotationAxis(*data.rotationAxis);
                if (data.totalAngle) animation->setTotalAngle(*data.totalAngle);
                if (data.useFocusOrigin) animation->setUseFocusOrigin(*data.useFocusOrigin);
                return animation;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::ScalingAnimationSpec>) {
                auto animation = std::make_shared<animation::Scaling>(data.mode);
                if (data.duration) animation->setDuration(*data.duration);
                if (data.targetScale) animation->setTargetScale(*data.targetScale);
                return animation;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::TranslationAnimationSpec>) {
                auto animation = std::make_shared<animation::Translation>(data.mode);
                if (data.duration) animation->setDuration(*data.duration);
                if (data.direction) animation->setDirection(*data.direction);
                if (data.totalDistance) animation->setTotalDistance(*data.totalDistance);
                return animation;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::KeyframeAnimationSpec>) {
                auto animation = std::make_shared<animation::KeyframeAnimation>(data.mode);
                if (data.duration) animation->setDuration(*data.duration);
                for (const auto& keyframe : data.keyframes) {
                    animation->addKeyframe(keyframe.timestamp, keyframe.transform);
                }
                return animation;
            }
            else if constexpr (std::is_same_v<T, pixelforge::builders::specification::CurveAnimationSpec>) {
                auto animation = std::make_shared<animation::CurveAnimation>(data.mode);
                if (data.duration) animation->setDuration(*data.duration);
                if (data.curve) animation->setCurve(data.curve);
                return animation;
            }
            else static_assert(always_false_v<T>, "Unhandled animation spec type");

        }, spec.data);
    }

} // namespace pixelforge::scene::builders::compiler
