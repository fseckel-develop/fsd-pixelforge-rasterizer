#pragma once
#include "Transform.h"
#include <GLM/glm.hpp>


/// Represents a pure translation as a wrapper around a transform.
class Translation final {
public:
    /// Default constructor initializing an identity transform.
    Translation() = default;

    /// Constructs a translation from a translation vector.
    /// @param translation The translation vector.
    explicit Translation(const glm::vec3& translation);

    /// Constructs a translation from individual distances.
    /// @param translateX Translation along the X-axis.
    /// @param translateY Translation along the Y-axis.
    /// @param translateZ Translation along the Z-axis.
    Translation(float translateX, float translateY, float translateZ);

    /// Provides the underlying translating transform for usage.
    /// @return Transform representing the translation.
    operator Transform() const { return translation; } // NOLINT

private:
    Transform translation; ///< The underlying translating transform.
};
