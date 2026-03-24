#pragma once
#include <pixelforge/scene/animation/animation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <vector>


namespace pixelforge::scene::animation {

    /// Represents a single keyframe in a KeyframeAnimation.
    typedef struct Keyframe {
        float timestamp = 0.0f; ///< Time (in seconds) at which this keyframe occurs.
        transform::Transform transform; ///< Transform state at this keyframe.

        /// Equality operator to compare keyframes.
        /// @param other The keyframe to compare against.
        /// @return True if both timestamps and all transform components are equal.
        bool operator==(const Keyframe& other) const {
            return timestamp == other.timestamp &&
                   transform.getTranslation() == other.transform.getTranslation() &&
                   transform.getRotation() == other.transform.getRotation() &&
                   transform.getScale() == other.transform.getScale();
        }
    } Keyframe;


    /// Extension of the Animation class which moves an
    /// object along discrete keyframes using interpolation.
    class KeyframeAnimation final : public Animation {
    public:
        /// Constructs a keyframe animation with a specified mode.
        /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
        explicit KeyframeAnimation(Mode mode);

        /// Adds a new keyframe to the animation at a given timestamp.
        /// @param timestamp The time (in seconds) of the keyframe.
        /// @param transform The Transform to apply at that timestamp.
        void addKeyframe(float timestamp, const transform::Transform& transform);

        /// Retrieves the full list of keyframes in this animation.
        /// @return Reference to the vector of keyframes.
        [[nodiscard]] const std::vector<Keyframe>& getKeyframes() const;

        /// Computes the current interpolated transform based on the animation progress.
        /// @return Transform that blends between the two surrounding keyframes.
        [[nodiscard]] transform::Transform getOffset() override;

    private:
        std::vector<Keyframe> keyframes_; ///< List of keyframes defining the animation.

        /// Interpolates between keyframes based on the current normalized time.
        /// @param progress Normalized animation progress in the range [0, 1].
        /// @return Transform object as mixture of the two enclosing keyframes.
        [[nodiscard]] transform::Transform interpolate(float progress) const;
    };

} // pixelforge::scene::animation
