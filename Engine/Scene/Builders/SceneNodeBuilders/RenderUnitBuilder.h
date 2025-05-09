#pragma once
#include "TransformNodeBuilder.h"
#include "../../SceneNodes/RenderUnit.h"
#include "../../../Geometry/Meshes/Mesh.h"
#include "../../../Graphics/Texturing/Material.h"


/// Extension of the TransformNodeBuilder class for
/// building a render unit using a fluent interface.
class RenderUnitBuilder final : public TransformNodeBuilder<RenderUnit, RenderUnitBuilder> {
public:
    /// Starts the building process for a render unit with a name and parent (optional).
    /// @param name The name of the render unit node.
    /// @param parentName The name of the parent node (optional).
    explicit RenderUnitBuilder(const std::string& name, const std::string& parentName = ""):
        TransformNodeBuilder(name, parentName) {
    }

    /// Sets the mesh for the render unit being built.
    /// @param mesh Shared pointer to the mesh to be set.
    /// @return Reference to the current builder for fluent chaining.
    auto& withMesh(const std::shared_ptr<Mesh>& mesh) {
        this->sceneNode->SetMesh(mesh);
        return *this;
    }

    /// Sets the mesh for the render unit being built.
    /// @param mesh Reference to the mesh to be set.
    /// @return Reference to the current builder for fluent chaining.
    auto& withMesh(const Mesh& mesh) {
        this->sceneNode->SetMesh(std::make_shared<Mesh>(mesh));
        return *this;
    }

    /// Loads a mesh from a file and adds it to the render unit being built.
    /// @param fileName The name of the .obj-file defining the mesh.
    /// @return Reference to the current builder for fluent chaining.
    auto& withMesh(const std::string& fileName) {
        this->sceneNode->SetMesh(std::make_shared<Mesh>(fileName));
        return *this;
    }

    /// Sets the material for the render unit being built.
    /// @param material Shared pointer to the material to be set.
    /// @return Reference to the current builder for fluent chaining.
    auto& withMaterial(const std::shared_ptr<Material>& material) {
        this->sceneNode->SetMaterial(material);
        return *this;
    }

    /// Sets the material for the render unit being built.
    /// @param material The material to be set.
    /// @return Reference to the current builder for fluent chaining.
    auto& withMaterial(const Material& material) {
        this->sceneNode->SetMaterial(std::make_shared<Material>(material));
        return *this;
    }

    /// Sets a texture-based material for the render unit being built.
    /// @param fileName The name of the texture file.
    /// @return Reference to the current builder for fluent chaining.
    auto& WithTexture(const std::string& fileName) {
        this->sceneNode->SetTexture(fileName);
        return *this;
    }
};


/// Alias function to start building a render unit.
/// @param name The name of the render unit.
/// @param parentName The optional name of the parent node.
/// @return Render unit builder for fluent configuration.
inline auto RenderUnit_(const std::string& name, const std::string& parentName = "") {
    return RenderUnitBuilder(name, parentName);
}
