#pragma once
#include <pixelforge/scene/lighting/light.hpp>
#include <GLM/glm.hpp>


namespace pixelforge::scene::lighting {

    /// Represents attenuation parameters for light intensity decrease.
    struct Attenuation {
        float constant;  ///< Constant attenuation term.
        float linear;    ///< Linear attenuation term.
        float quadratic; ///< Quadratic attenuation term.
    };


    /// Predefined effective light ranges, mappable to specific attenuation values.
    enum Range {
        R3250, R600, R325, R200, R160, R100, R65, R50, R32, R20, R13, R7
    };


    /// Extension of the Light class with a specified position and attenuation behavior.
    class PositionalLight : public Light {
    public:
        /// Default constructor located at (0, 0, 0) with a range of R100.
        PositionalLight();

        /// Constructs a positional light at a specified position.
        /// @param position The world-space position of the light.
        explicit PositionalLight(const glm::vec3& position);

        /// Sets the position of the light.
        /// @param position The new world-space position of the light.
        void setPosition(const glm::vec3& position);

        /// Sets the attenuation factors manually.
        /// @param attenuation Attenuation structure defining falloff behavior.
        void setAttenuation(const Attenuation& attenuation);

        /// Sets the attenuation by using a predefined range.
        /// @param range A range value representing effective radius.
        void setRange(Range range);

        /// Gets the current position of the light.
        /// @return 3D vector for the light's position.
        [[nodiscard]] glm::vec3 getPosition() const override;

        /// Gets the current attenuation parameters.
        /// @return An attenuation structure.
        [[nodiscard]] Attenuation getAttenuation() const;

    protected:
        glm::vec3 position_; ///< Position of the light in world space.
        Attenuation attenuation_; ///< Attenuation parameters determining lighting falloff.

        /// Helper function to map a given range to an attenuation.
        /// @param range The range value.
        /// @return Corresponding attenuation structure.
        static Attenuation getAttenuationForRange(Range range);
    };

} // namespace pixelforge::scene::lighting
