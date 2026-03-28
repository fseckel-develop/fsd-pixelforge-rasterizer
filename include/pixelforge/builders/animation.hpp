#pragma once
#include <pixelforge/builders/specification/animation_spec.hpp>


namespace pixelforge::builders {

    /// Common base class for declarative animation builders.
    /// Provides shared helper functionality for assigning duration
    /// and finalizing the build process into an AnimationSpec.
    class AnimationBuilderBase {
    public:
        /// Finalizes the build process and returns the animation specification.
        /// @return Declarative specification of the configured animation.
        [[nodiscard]] specification::AnimationSpec buildSpec() const {
            return spec_;
        }

    protected:
        specification::AnimationSpec spec_; ///< The underlying animation specification being built.

        /// Sets the duration of the animation being built.
        /// @tparam AnimationSpecT The concrete animation specification type.
        /// @tparam DerivedT The concrete builder type.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        template<typename AnimationSpecT, typename DerivedT>
        auto& setDuration(const float duration) {
            std::get<AnimationSpecT>(spec_.data).duration = duration;
            return static_cast<DerivedT&>(*this);
        }
    };


    /// Fluent builder for constructing a rotation animation specification.
    class Rotation final : public AnimationBuilderBase {
    public:
        /// Starts the building process for a rotation animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit Rotation(const scene::animation::Animation::Mode mode) {
            specification::RotationAnimationSpec data{};
            data.mode = mode;
            spec_.data = data;
        }

        /// Sets the duration of the rotation animation being built.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        auto& duration(const float duration) {
            return setDuration<specification::RotationAnimationSpec, Rotation>(duration);
        }

        /// Sets the rotation axis of the rotation animation being built.
        /// @param rotationAxis The rotation axis vector.
        /// @return Reference to this builder for method chaining.
        auto& rotationAxis(const glm::vec3& rotationAxis) {
            std::get<specification::RotationAnimationSpec>(spec_.data).rotationAxis = rotationAxis;
            return *this;
        }

        /// Sets the total angle of the rotation animation being built.
        /// @param totalAngle The total angle in degrees.
        /// @return Reference to this builder for method chaining.
        auto& totalAngle(const float totalAngle) {
            std::get<specification::RotationAnimationSpec>(spec_.data).totalAngle = totalAngle;
            return *this;
        }
    };


    /// Fluent builder for constructing an orbiting animation specification.
    class Orbiting final : public AnimationBuilderBase {
    public:
        /// Starts the building process for an orbiting animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit Orbiting(const scene::animation::Animation::Mode mode) {
            specification::OrbitingAnimationSpec data{};
            data.mode = mode;
            spec_.data = data;
        }

        /// Sets the duration of the orbiting animation being built.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        auto& duration(const float duration) {
            return setDuration<specification::OrbitingAnimationSpec, Orbiting>(duration);
        }

        /// Sets the orbit radius of the orbiting animation being built.
        /// @param radius The orbit radius.
        /// @return Reference to this builder for method chaining.
        auto& radius(const float radius) {
            std::get<specification::OrbitingAnimationSpec>(spec_.data).radius = radius;
            return *this;
        }

        /// Sets the rotation axis of the orbiting animation being built.
        /// @param rotationAxis The rotation axis vector.
        /// @return Reference to this builder for method chaining.
        auto& rotationAxis(const glm::vec3& rotationAxis) {
            std::get<specification::OrbitingAnimationSpec>(spec_.data).rotationAxis = rotationAxis;
            return *this;
        }

        /// Sets the total revolution angle of the orbiting animation being built.
        /// @param totalAngle The total angle in degrees.
        /// @return Reference to this builder for method chaining.
        auto& totalAngle(const float totalAngle) {
            std::get<specification::OrbitingAnimationSpec>(spec_.data).totalAngle = totalAngle;
            return *this;
        }
    };


    /// Fluent builder for constructing an elliptic orbiting animation specification.
    class EllipticOrbiting final : public AnimationBuilderBase {
    public:
        /// Starts the building process for an elliptic orbiting animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit EllipticOrbiting(const scene::animation::Animation::Mode mode) {
            specification::EllipticOrbitingAnimationSpec data{};
            data.mode = mode;
            spec_.data = data;
        }

        /// Sets the duration of the elliptic orbiting animation being built.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        auto& duration(const float duration) {
            return setDuration<specification::EllipticOrbitingAnimationSpec, EllipticOrbiting>(duration);
        }

        /// Sets the semi-major axis of the elliptic orbit being built.
        /// @param semiMajorAxis The semi-major axis length.
        /// @return Reference to this builder for method chaining.
        auto& semiMajorAxis(const float semiMajorAxis) {
            std::get<specification::EllipticOrbitingAnimationSpec>(spec_.data).semiMajorAxis = semiMajorAxis;
            return *this;
        }

        /// Sets the semi-minor axis of the elliptic orbit being built.
        /// @param semiMinorAxis The semi-minor axis length.
        /// @return Reference to this builder for method chaining.
        auto& semiMinorAxis(const float semiMinorAxis) {
            std::get<specification::EllipticOrbitingAnimationSpec>(spec_.data).semiMinorAxis = semiMinorAxis;
            return *this;
        }

        /// Sets both ellipse axes of the elliptic orbit being built.
        /// @param semiMajorAxis The semi-major axis length.
        /// @param semiMinorAxis The semi-minor axis length.
        /// @return Reference to this builder for method chaining.
        auto& axes(const float semiMajorAxis, const float semiMinorAxis) {
            auto& data = std::get<specification::EllipticOrbitingAnimationSpec>(spec_.data);
            data.semiMajorAxis = semiMajorAxis;
            data.semiMinorAxis = semiMinorAxis;
            return *this;
        }

        /// Sets the rotation axis of the elliptic orbiting animation being built.
        /// @param rotationAxis The rotation axis vector.
        /// @return Reference to this builder for method chaining.
        auto& rotationAxis(const glm::vec3& rotationAxis) {
            std::get<specification::EllipticOrbitingAnimationSpec>(spec_.data).rotationAxis = rotationAxis;
            return *this;
        }

        /// Sets the total revolution angle of the elliptic orbiting animation being built.
        /// @param totalAngle The total angle in degrees.
        /// @return Reference to this builder for method chaining.
        auto& totalAngle(const float totalAngle) {
            std::get<specification::EllipticOrbitingAnimationSpec>(spec_.data).totalAngle = totalAngle;
            return *this;
        }

        /// Sets whether the orbit origin should lie at a focus instead of the ellipse center.
        /// @param useFocusOrigin True to use a focus-based origin, false for a centered ellipse.
        /// @return Reference to this builder for method chaining.
        auto& useFocusOrigin(const bool useFocusOrigin) {
            std::get<specification::EllipticOrbitingAnimationSpec>(spec_.data).useFocusOrigin = useFocusOrigin;
            return *this;
        }
    };


    /// Fluent builder for constructing a scaling animation specification.
    class Scaling final : public AnimationBuilderBase {
    public:
        /// Starts the building process for a scaling animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit Scaling(const scene::animation::Animation::Mode mode) {
            specification::ScalingAnimationSpec data{};
            data.mode = mode;
            spec_.data = data;
        }

        /// Sets the duration of the scaling animation being built.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        auto& duration(const float duration) {
            return setDuration<specification::ScalingAnimationSpec, Scaling>(duration);
        }

        /// Sets a uniform target scale for the scaling animation being built.
        /// @param scale The target scale factor for all axes.
        /// @return Reference to this builder for method chaining.
        auto& targetScale(const float scale) {
            std::get<specification::ScalingAnimationSpec>(spec_.data).targetScale = glm::vec3(scale);
            return *this;
        }

        /// Sets a non-uniform target scale vector for the scaling animation being built.
        /// @param scale The 3D scale vector.
        /// @return Reference to this builder for method chaining.
        auto& targetScale(const glm::vec3& scale) {
            std::get<specification::ScalingAnimationSpec>(spec_.data).targetScale = scale;
            return *this;
        }
    };


    /// Fluent builder for constructing a translation animation specification.
    class Translation final : public AnimationBuilderBase {
    public:
        /// Starts the building process for a translation animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit Translation(const scene::animation::Animation::Mode mode) {
            specification::TranslationAnimationSpec data{};
            data.mode = mode;
            spec_.data = data;
        }

        /// Sets the duration of the translation animation being built.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        auto& duration(const float duration) {
            return setDuration<specification::TranslationAnimationSpec, Translation>(duration);
        }

        /// Sets the translation direction of the animation being built.
        /// @param direction The translation direction vector.
        /// @return Reference to this builder for method chaining.
        auto& direction(const glm::vec3& direction) {
            std::get<specification::TranslationAnimationSpec>(spec_.data).direction = direction;
            return *this;
        }

        /// Sets the total translation distance of the animation being built.
        /// @param totalDistance The total translation distance.
        /// @return Reference to this builder for method chaining.
        auto& totalDistance(const float totalDistance) {
            std::get<specification::TranslationAnimationSpec>(spec_.data).totalDistance = totalDistance;
            return *this;
        }
    };


    /// Fluent builder for constructing a keyframe animation specification.
    class KeyframeAnimation final : public AnimationBuilderBase {
    public:
        /// Starts the building process for a keyframe animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit KeyframeAnimation(const scene::animation::Animation::Mode mode) {
            specification::KeyframeAnimationSpec data{};
            data.mode = mode;
            spec_.data = data;
        }

        /// Sets the duration of the keyframe animation being built.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        auto& duration(const float duration) {
            return setDuration<specification::KeyframeAnimationSpec, KeyframeAnimation>(duration);
        }

        /// Adds a keyframe to the animation being built.
        /// @param timestamp The timestamp of the keyframe in seconds.
        /// @param transform The transform at the given timestamp.
        /// @return Reference to this builder for method chaining.
        auto& keyframe(const float timestamp, const scene::transform::Transform& transform) {
            std::get<specification::KeyframeAnimationSpec>(spec_.data).keyframes.push_back({timestamp, transform});
            return *this;
        }

        /// Adds a prebuilt keyframe to the animation being built.
        /// @param keyframe The keyframe to add.
        /// @return Reference to this builder for method chaining.
        auto& keyframe(const scene::animation::Keyframe& keyframe) {
            std::get<specification::KeyframeAnimationSpec>(spec_.data).keyframes.push_back(keyframe);
            return *this;
        }
    };


    /// Fluent builder for constructing a curve animation specification.
    class CurveAnimation final : public AnimationBuilderBase {
    public:
        /// Starts the building process for a curve animation.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit CurveAnimation(const scene::animation::Animation::Mode mode) {
            specification::CurveAnimationSpec data{};
            data.mode = mode;
            spec_.data = data;
        }

        /// Sets the duration of the curve animation being built.
        /// @param duration The duration in seconds.
        /// @return Reference to this builder for method chaining.
        auto& duration(const float duration) {
            return setDuration<specification::CurveAnimationSpec, CurveAnimation>(duration);
        }

        /// Assigns the curve along which the animation moves the object.
        /// @param curve Shared pointer to a curve representing the trajectory.
        /// @return Reference to this builder for method chaining.
        auto& curve(const std::shared_ptr<geometry::Curve>& curve) {
            std::get<specification::CurveAnimationSpec>(spec_.data).curve = curve;
            return *this;
        }
    };

} // namespace pixelforge::builders
