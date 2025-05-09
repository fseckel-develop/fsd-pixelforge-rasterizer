#pragma once
#include "TransformNodeBuilder.h"
#include "../../SceneNodes/LightUnit.h"


/// Extension of the TransformNodeBuilder class for
/// building a light unit using a fluent interface.
class LightUnitBuilder final : public TransformNodeBuilder<LightUnit, LightUnitBuilder> {
public:
    /// Starts the building process for a light unit with a name and parent (optional).
    /// @param name The name of the light unit.
    /// @param parentName The name of the parent node (optional).
    explicit LightUnitBuilder(const std::string& name, const std::string& parentName = ""):
        TransformNodeBuilder(name, parentName) {
    }

    /// Adds a light to the light unit being built.
    /// @param light Shared pointer to the light to be set.
    /// @return Reference to the current builder for fluent chaining.
    auto& withLight(const std::shared_ptr<Light>& light) {
        this->sceneNode->SetLight(light);
        return *this;
    }

    /// @brief Adds a light to the light unit using a light builder.
    /// @param builder Light builder to create and set the light.
    /// @tparam BuilderT The type of the light builder.
    /// @return Reference to the current builder for fluent chaining.
    template<IsLightBuilder BuilderT>
    auto& withLight(const BuilderT& builder) {
        this->sceneNode->SetLight(builder.Build());
        return *this;
    }

    /// Adds a mesh to the light unit being built.
    /// @param mesh Shared pointer to the mesh to be set.
    /// @return Reference to the current builder for fluent chaining.
    auto& hasMesh(const std::shared_ptr<Mesh>& mesh) {
        this->sceneNode->SetMesh(mesh);
        return *this;
    }
};


/// Alias function to start building a light unit.
/// @param name The name of the light unit.
/// @param parentName The optional name of the parent node.
/// @return Light unit builder for fluent configuration.
inline auto LightUnit_(const std::string& name, const std::string& parentName = "") {
    return LightUnitBuilder(name, parentName);
}
