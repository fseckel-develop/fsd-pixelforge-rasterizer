#pragma once
#include "PositionalLight.h"
#include <GLM/glm.hpp>


/// Extension of the PositionalLight class with directionality and an inner
/// and outer cutoff angle, emitting only within a cone-shaped volume.
class SpotLight final : public PositionalLight {
public:
    /// Default constructor, with the direction set
    /// to (0, -1, 0) and cutoff between 5° and 15°.
    SpotLight();

    /// Constructs a spotlight with a position and direction (cutoff between 5° and 15°).
    /// @param position The world position of the spotlight.
    /// @param direction The direction the spotlight is facing.
    SpotLight(const glm::vec3& position, const glm::vec3& direction);

    /// Sets the direction the spotlight is pointing.
    /// @param direction Normalized direction vector.
    void SetDirection(const glm::vec3& direction);

    /// Sets the inner and outer cutoff angles.
    /// @param innerCutoff The angle of the fully lit cone.
    /// @param outerCutoff The angle beyond which light is fully attenuated.
    void SetCutoffAngles(float innerCutoff, float outerCutoff);

    /// Gets the current spotlight direction.
    /// @return Normalized direction vector.
    [[nodiscard]] glm::vec3 GetDirection() const override;

    /// Gets the cosine of the inner cutoff angle.
    /// @return Cosine value.
    [[nodiscard]] float GetInnerCutoff() const;

    /// Gets the cosine of the outer cutoff angle.
    /// @return Cosine value.
    [[nodiscard]] float GetOuterCutoff() const;

private:
    glm::vec3 direction; ///< The normalized direction the spotlight is facing.
    float innerCutoff;   ///< Cosine of the inner cutoff angle (fully lit cone).
    float outerCutoff;   ///< Cosine of the outer cutoff angle (fully attenuated).
};
