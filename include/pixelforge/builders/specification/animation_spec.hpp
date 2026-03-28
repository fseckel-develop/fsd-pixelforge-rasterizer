#pragma once
#include <pixelforge/geometry/curves/curve.hpp>
#include <pixelforge/scene/animation/animation.hpp>
#include <pixelforge/scene/animation/keyframe_animation.hpp>
#include <optional>
#include <variant>
#include <vector>


namespace pixelforge::builders::specification {

    /// Declarative specification for constructing a rotation animation.
    struct RotationAnimationSpec {
        scene::animation::Animation::Mode mode; ///< Playback mode.
        std::optional<float> duration; ///< Optional duration in seconds.
        std::optional<glm::vec3> rotationAxis; ///< Optional rotation axis.
        std::optional<float> totalAngle; ///< Optional total angle in degrees.
    };

    /// Declarative specification for constructing an orbiting animation.
    struct OrbitingAnimationSpec {
        scene::animation::Animation::Mode mode; ///< Playback mode.
        std::optional<float> duration; ///< Optional duration in seconds.
        std::optional<float> radius; ///< Optional orbit radius.
        std::optional<glm::vec3> rotationAxis; ///< Optional orbit axis.
        std::optional<float> totalAngle; ///< Optional total revolution angle in degrees.
    };

    /// Declarative specification for constructing an elliptic orbiting animation.
    struct EllipticOrbitingAnimationSpec {
        scene::animation::Animation::Mode mode; ///< Playback mode.
        std::optional<float> duration; ///< Optional duration in seconds.
        std::optional<float> semiMajorAxis; ///< Optional semi-major axis length.
        std::optional<float> semiMinorAxis; ///< Optional semi-minor axis length.
        std::optional<glm::vec3> rotationAxis; ///< Optional orbit axis.
        std::optional<float> totalAngle; ///< Optional total revolution angle in degrees.
        std::optional<bool> useFocusOrigin; ///< Optional flag for placing the orbit origin at a focus.
    };

    /// Declarative specification for constructing a scaling animation.
    struct ScalingAnimationSpec {
        scene::animation::Animation::Mode mode; ///< Playback mode.
        std::optional<float> duration; ///< Optional duration in seconds.
        std::optional<glm::vec3> targetScale; ///< Optional target scale vector.
    };

    /// Declarative specification for constructing a translation animation.
    struct TranslationAnimationSpec {
        scene::animation::Animation::Mode mode; ///< Playback mode.
        std::optional<float> duration; ///< Optional duration in seconds.
        std::optional<glm::vec3> direction; ///< Optional translation direction.
        std::optional<float> totalDistance; ///< Optional total translation distance.
    };

    /// Declarative specification for constructing a keyframe animation.
    struct KeyframeAnimationSpec {
        scene::animation::Animation::Mode mode; ///< Playback mode.
        std::optional<float> duration; ///< Optional duration in seconds.
        std::vector<scene::animation::Keyframe> keyframes; ///< Ordered keyframes of the animation.
    };

    /// Declarative specification for constructing a curve animation.
    struct CurveAnimationSpec {
        scene::animation::Animation::Mode mode; ///< Playback mode.
        std::optional<float> duration; ///< Optional duration in seconds.
        std::shared_ptr<geometry::Curve> curve = nullptr; ///< Curve followed by the animation.
    };

    /// Declarative specification for constructing an animation.
    struct AnimationSpec {
        using Data = std::variant<
            RotationAnimationSpec,
            OrbitingAnimationSpec,
            EllipticOrbitingAnimationSpec,
            ScalingAnimationSpec,
            TranslationAnimationSpec,
            KeyframeAnimationSpec,
            CurveAnimationSpec
        >;

        Data data; ///< Type-specific animation specification payload.
    };

} // namespace pixelforge::builders::specification
