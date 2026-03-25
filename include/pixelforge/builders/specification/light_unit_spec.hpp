#pragma once
#include <pixelforge/builders/specification/light_spec.hpp>
#include <pixelforge/builders/specification/animation_spec.hpp>
#include <pixelforge/geometry/meshes/mesh.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <optional>
#include <string>
#include <vector>


namespace pixelforge::builders::specification {

    /// Declarative specification for constructing a LightUnit.
    /// Stores all configuration data needed to create the runtime node later.
    struct LightUnitSpec {
        std::string name; ///< Name of the light unit.
        std::string parentName; ///< Optional parent node name.

        std::optional<LightSpec> light; ///< Optional light assigned to the light unit.
        std::shared_ptr<geometry::Mesh> mesh = nullptr; ///< Optional mesh used to visualize the light.

        std::optional<scene::transform::Transform> transform; ///< Optional transform.
        std::optional<float> uniformNodeScale; ///< Optional uniform node scale.
        std::optional<glm::vec3> nodeScale; ///< Optional non-uniform node scale.

        std::vector<AnimationSpec> animations; ///< Animations applied to this light unit.

        /// Creates a LightUnit specification with a required name.
        /// @param name Name of the light unit.
        explicit LightUnitSpec(const std::string& name)
            : name(name) {}
    };

} // namespace pixelforge::builders::specification
