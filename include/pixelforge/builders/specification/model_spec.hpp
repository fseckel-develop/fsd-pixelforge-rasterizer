#pragma once
#include <pixelforge/builders/specification/animation_spec.hpp>
#include <pixelforge/builders/specification/light_unit_spec.hpp>
#include <pixelforge/builders/specification/render_unit_spec.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <optional>
#include <string>
#include <vector>


namespace pixelforge::builders::specification {

    /// Declarative specification for constructing a Model.
    /// Stores child light/render units and transform-related configuration.
    struct ModelSpec {
        std::string name; ///< Name of the model.
        std::string parentName; ///< Optional parent node name.

        std::optional<scene::transform::Transform> transform; ///< Optional transform.
        std::optional<float> uniformNodeScale; ///< Optional uniform node scale.
        std::optional<glm::vec3> nodeScale; ///< Optional non-uniform node scale.

        std::vector<AnimationSpec> animations; ///< Animations applied to this model.
        std::vector<LightUnitSpec> lightUnits; ///< Light units contained in the model.
        std::vector<RenderUnitSpec> renderUnits; ///< Render units contained in the model.

        /// Creates a Model specification with a required name.
        /// @param name Name of the model.
        explicit ModelSpec(const std::string& name)
            : name(name) {}
    };

} // namespace pixelforge::builders::specification
