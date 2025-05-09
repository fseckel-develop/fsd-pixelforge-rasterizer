#pragma once
#include "Animation.h"
#include <GLM/glm.hpp>
class Transform;


/// Extension of the Animation class which translates
/// an object around a specified axis over time.
class Translating final : public Animation {
public:
    /// Constructs a translating animation with a specified mode,
    /// default distance of 1.0 and default direction (0, 0, 1).
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    explicit Translating(Mode mode);

    /// Constructs a translating animation with custom values.
    /// @param mode The playback mode (ONCE, LOOP, BOUNCE).
    /// @param duration Total duration of the animation in seconds.
    /// @param direction Unit vector representing translation direction.
    /// @param distance Total distance to translate over the animation's duration.
    Translating(Mode mode, float duration, const glm::vec3& direction, float distance);

    /// Sets the total distance to translate.
    /// @param distance The full movement distance.
    void SetTotalDistance(float distance);

    /// Sets the direction of translation.
    /// @param direction 3D vector direction to move along.
    void SetDirection(const glm::vec3& direction);

    /// Gets the total distance of the translation.
    /// @return Distance scalar value.
    [[nodiscard]] float GetTotalDistance() const;

    /// Gets the direction vector for the translation.
    /// @return Reference to a unit vector representing direction.
    [[nodiscard]] const glm::vec3& GetDirection() const;

    /// Calculates the current translation offset based on the animation progress.
    /// @return Transform containing the progress-weighted translation.
    [[nodiscard]] Transform GetOffset() override;

private:
    glm::vec3 direction; ///< Direction in which the translation occurs.
    float totalDistance; ///< Total distance reached at 100% animation progress.
};
