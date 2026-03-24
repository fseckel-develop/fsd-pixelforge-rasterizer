#pragma once
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>


namespace pixelforge::scene::transform {

    /// Represents a pure translation as a wrapper around a transform.
    class Translate final {
    public:
        /// Default constructor initializing an identity transform.
        Translate() = default;

        /// Constructs a translation from a translation vector.
        /// @param translation The translation vector.
        explicit Translate(const glm::vec3& translation);

        /// Constructs a translation from individual distances.
        /// @param translateX Translation along the X-axis.
        /// @param translateY Translation along the Y-axis.
        /// @param translateZ Translation along the Z-axis.
        Translate(float translateX, float translateY, float translateZ);

        /// Provides the underlying translating transform for usage.
        /// @return Transform representing the translation.
        operator Transform() const { return translation_; } // NOLINT

    private:
        Transform translation_; ///< The underlying translating transform.
    };

} // namespace pixelforge::scene::transform
