#pragma once
#include "Animation.h"
#include "../Transforms/Transform.h"
#include <vector>


/// Represents a single keyframe in a KeyframeAnimation.
typedef struct Keyframe {
    float timestamp = 0.0f; ///< Time (in seconds) at which this keyframe occurs.
    Transform transform;    ///< Transform state at this keyframe.

    /// Equality operator to compare keyframes.
    /// @param other The keyframe to compare against.
    /// @return True if both timestamps and all transform components are equal.
    bool operator==(const Keyframe& other) const {
        return timestamp == other.timestamp &&
               transform.GetTranslation() == other.transform.GetTranslation() &&
               transform.GetRotation() == other.transform.GetRotation() &&
               transform.GetScale() == other.transform.GetScale();
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
    void AddKeyframe(float timestamp, const Transform& transform);

    /// Retrieves the full list of keyframes in this animation.
    /// @return Reference to the vector of keyframes.
    [[nodiscard]] const std::vector<Keyframe>& GetKeyframes() const;

    /// Computes the current interpolated transform based on the animation progress.
    /// @return Transform that blends between the two surrounding keyframes.
    [[nodiscard]] Transform GetOffset() override;

private:
    std::vector<Keyframe> keyframes; ///< List of keyframes defining the animation.

    /// Interpolates between keyframes based on the current normalized time.
    /// @param progress Normalized animation progress in the range [0, 1].
    /// @return Transform object as mixture of the two enclosing keyframes.
    [[nodiscard]] Transform Interpolate(float progress) const;
};