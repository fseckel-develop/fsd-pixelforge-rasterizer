#pragma once
#include <pixelforge/builders/specification/animation_spec.hpp>
#include <pixelforge/geometry/meshes/mesh.hpp>
#include <pixelforge/graphics/texturing/material.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <optional>
#include <string>
#include <vector>


namespace pixelforge::builders::specification {

    /// Declarative specification for constructing a RenderUnit.
    /// Stores all configuration data needed to create the runtime node later.
    struct RenderUnitSpec {
        std::string name; ///< Name of the render unit.
        std::string parentName; ///< Optional parent node name.

        std::shared_ptr<geometry::Mesh> mesh = nullptr; ///< Optional mesh instance.
        std::string meshFileName; ///< Optional .obj mesh file name.

        std::shared_ptr<graphics::Material> material = nullptr; ///< Optional material instance.
        std::string textureFileName; ///< Optional texture file name.

        std::optional<scene::transform::Transform> transform; ///< Optional transform.
        std::optional<float> uniformNodeScale; ///< Optional uniform node scale.
        std::optional<glm::vec3> nodeScale; ///< Optional non-uniform node scale.

        std::vector<AnimationSpec> animations; ///< Animations applied to this render unit.

        /// Creates a RenderUnit specification with a required name.
        /// @param name Name of the render unit.
        explicit RenderUnitSpec(const std::string& name)
            : name(name) {}
    };

} // namespace pixelforge::builders::specification
