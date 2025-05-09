#pragma once
#include "GroupBuilder.h"
#include "ModelBuilder.h"
#include "LightUnitBuilder.h"
#include "../../SceneNodes/Scene.h"


/// Extension of the GroupBuilder class for
/// building a scene using a fluent interface.
class SceneBuilder final : public GroupBuilder<Scene, SceneBuilder> {
public:
    /// Starts the building process for a scene with a name.
    /// @param name The name of the Scene.
    explicit SceneBuilder(const std::string& name):
        GroupBuilder(name) {
    }

    /// Adds a light unit to the scene using a light unit builder.
    /// @param builder Light unit builder to create and set the light unit.
    /// @return Reference to the current builder for fluent chaining.
    auto& With(const LightUnitBuilder& builder) {
        this->sceneNode->AddLightUnit(builder.Build());
        return *this;
    }

    /// Adds a model to the scene using a model builder.
    /// @param builder Model builder to create and set the model.
    /// @return Reference to the current builder for fluent chaining.
    auto& With(const ModelBuilder& builder) {
        this->sceneNode->AddModel(builder.Build());
        return *this;
    }
};


/// Alias function to start building a scene.
/// @param name The name of the scene.
/// @return Scene builder for fluent configuration.
inline auto Scene_(const std::string& name) {
    return SceneBuilder(name);
}
