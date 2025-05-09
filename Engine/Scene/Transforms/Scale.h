#pragma once
#include <GLM/glm.hpp>
#include "Transform.h"


/// Represents a pure scale as a wrapper around a transform.
class Scale final {
public:
    /// Default constructor initializing an identity transform.
    Scale() = default;

    /// Constructs a scale from a uniform scale factor.
    /// @param scale The uniform scale factor.
    explicit Scale(float scale);
    explicit Scale(const glm::vec3&);

    /// Constructs a scale from individual scale factors.
    /// @param scaleX Scale factor along X-axis.
    /// @param scaleY Scale factor along Y-axis.
    /// @param scaleZ Scale factor along Z-axis.
    Scale(float scaleX, float scaleY, float scaleZ);

    /// Provides the underlying scaling transform for usage.
    /// @return Transform representing the scale.
    operator Transform() const { return scale; } // NOLINT

private:
    Transform scale; ///< The underlying scaling transform.
};
