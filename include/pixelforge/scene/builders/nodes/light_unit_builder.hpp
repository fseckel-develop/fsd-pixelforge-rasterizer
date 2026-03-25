#pragma once
#include <pixelforge/scene/builders/nodes/transform_node_builder.hpp>
#include <pixelforge/scene/builders/lighting/light_builder.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>


namespace pixelforge::scene::nodes {

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
        auto& withLight(const std::shared_ptr<lighting::Light>& light) {
            this->sceneNode_->setLight(light);
            return *this;
        }

        /// @brief Adds a light to the light unit using a light builder.
        /// @param builder Light builder to create and set the light.
        /// @tparam BuilderT The type of the light builder.
        /// @return Reference to the current builder for fluent chaining.
        template<lighting::IsLightBuilder BuilderT>
        auto& withLight(const BuilderT& builder) {
            this->sceneNode_->setLight(builder.build());
            return *this;
        }

        /// Adds a mesh to the light unit being built.
        /// @param mesh Shared pointer to the mesh to be set.
        /// @return Reference to the current builder for fluent chaining.
        auto& hasMesh(const std::shared_ptr<geometry::Mesh>& mesh) {
            this->sceneNode_->setMesh(mesh);
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

} // namespace pixelforge::scene::nodes
