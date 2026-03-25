#pragma once
#include <pixelforge/scene/lighting/light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <optional>
#include <variant>


namespace pixelforge::builders::specification {

    /// Declarative specification for constructing an ambient light.
    struct AmbientLightSpec {
        std::optional<scene::lighting::LightComponent> ambient;  ///< Optional ambient component override.
    };

    /// Declarative specification for constructing a directional light.
    struct DirectionalLightSpec {
        std::optional<scene::lighting::LightComponent> ambient;  ///< Optional ambient component override.
        std::optional<scene::lighting::LightComponent> diffuse;  ///< Optional diffuse component override.
        std::optional<scene::lighting::LightComponent> specular; ///< Optional specular component override.
        std::optional<glm::vec3> direction; ///< Optional direction override.
    };

    /// Declarative specification for constructing a positional light.
    struct PositionalLightSpec {
        std::optional<scene::lighting::LightComponent> ambient;  ///< Optional ambient component override.
        std::optional<scene::lighting::LightComponent> diffuse;  ///< Optional diffuse component override.
        std::optional<scene::lighting::LightComponent> specular; ///< Optional specular component override.
        std::optional<glm::vec3> position; ///< Optional position override.
        std::optional<scene::lighting::Attenuation> attenuation; ///< Optional explicit attenuation override.
        std::optional<scene::lighting::Range> range; ///< Optional range-based attenuation override.
    };

    /// Declarative specification for constructing a spotlight.
    struct SpotLightSpec {
        std::optional<scene::lighting::LightComponent> ambient;  ///< Optional ambient component override.
        std::optional<scene::lighting::LightComponent> diffuse;  ///< Optional diffuse component override.
        std::optional<scene::lighting::LightComponent> specular; ///< Optional specular component override.
        std::optional<glm::vec3> position;  ///< Optional position override.
        std::optional<glm::vec3> direction; ///< Optional direction override.
        std::optional<scene::lighting::Attenuation> attenuation; ///< Optional explicit attenuation override.
        std::optional<scene::lighting::Range> range; ///< Optional range-based attenuation override.
        std::optional<float> innerCutoff; ///< Optional inner cutoff angle in degrees.
        std::optional<float> outerCutoff; ///< Optional outer cutoff angle in degrees.
    };

    /// Declarative specification for constructing a light.
    struct LightSpec {
        using Data = std::variant<
            AmbientLightSpec,
            DirectionalLightSpec,
            PositionalLightSpec,
            SpotLightSpec
        >;

        Data data; ///< Type-specific light specification payload.
    };

} // namespace pixelforge::builders::specification
