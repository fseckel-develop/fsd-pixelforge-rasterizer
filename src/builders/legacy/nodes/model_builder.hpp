#pragma once
#include "transform_node_builder.hpp"
#include "light_unit_builder.hpp"
#include "render_unit_builder.hpp"
#include <pixelforge/scene/nodes/model.hpp>


namespace pixelforge::scene::nodes {

    /// Extension of the TransformNodeBuilder class for
    /// building a model using a fluent interface.
    class ModelBuilder final : public TransformNodeBuilder<Model, ModelBuilder> {
    public:
        /// Starts the building process for a model with a name and parent (optional).
        /// @param name The name of the model.
        /// @param parentName The name of the parent node (optional).
        explicit ModelBuilder(const std::string& name, const std::string& parentName = ""):
            TransformNodeBuilder(name, parentName) {
        }

        /// Adds a light unit to the model using a light unit builder.
        /// @param builder Light unit builder to create and set the light unit.
        /// @return Reference to the current builder for fluent chaining.
        auto& with(const LightUnitBuilder& builder) {
            this->sceneNode_->addLightUnit(builder.build());
            return *this;
        }

        /// Adds a render unit to the model using a render unit builder.
        /// @param builder Render unit builder to create and set the render unit.
        /// @return Reference to the current builder for fluent chaining.
        auto& with(const RenderUnitBuilder& builder) {
            this->sceneNode_->addRenderUnit(builder.build());
            return *this;
        }
    };


    /// Alias function to start building a model.
    /// @param name The name of the model.
    /// @param parentName The optional name of the parent node.
    /// @return Model builder for fluent configuration.
    inline auto Model_(const std::string& name, const std::string& parentName = "") {
        return ModelBuilder(name, parentName);
    }

} // namespace pixelforge::scene::nodes
