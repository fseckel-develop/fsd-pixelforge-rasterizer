#pragma once
#include <pixelforge/scene/nodes/transform_node.hpp>

namespace pixelforge::graphics { class Material; }
namespace pixelforge::geometry { class Mesh; }

namespace pixelforge::scene::nodes {

    /// Extension of the TransformNode class that represents a geometry
    /// that can be rendered, combining a mesh with material data.
    class RenderUnit final : public TransformNode {
    public:
        /// Constructs a render unit with a given name.
        /// @param name The name of the render unit.
        explicit RenderUnit(const std::string& name);

        /// Constructs a render unit with a given name and mesh.
        /// @param name The name of the render unit.
        /// @param mesh Shared pointer to the mesh to be used.
        RenderUnit(const std::string& name, const std::shared_ptr<geometry::Mesh>& mesh);

        /// Sets the mesh of the render unit.
        /// @param mesh Shared pointer to the new mesh to be used.
        void setMesh(const std::shared_ptr<geometry::Mesh>& mesh);

        /// Sets the material of the render unit.
        /// @param material Shared pointer to the new material to be used.
        void setMaterial(const std::shared_ptr<graphics::Material>& material);

        /// Sets the material of the render unit solely based on a given texture.
        /// @param textureFileName The file name of the texture used for the material.
        void setTexture(const std::string& textureFileName);

        /// Retrieves the mesh associated with the render unit.
        /// @return Reference to the shared pointer of the mesh.
        [[nodiscard]] const std::shared_ptr<geometry::Mesh>& getMesh() const;

        /// Retrieves the material associated with the render unit.
        /// @return Reference to the shared pointer of the material.
        [[nodiscard]] const std::shared_ptr<graphics::Material>& getMaterial() const;

    private:
        std::shared_ptr<geometry::Mesh> mesh_; ///< The mesh used by the render unit.
        std::shared_ptr<graphics::Material> material_; ///< The material used by the render unit.
    };

} // namespace pixelforge::scene::nodes
