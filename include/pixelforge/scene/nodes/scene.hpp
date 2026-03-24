#pragma once
#include <pixelforge/scene/nodes/group.hpp>
#include <vector>


namespace pixelforge::scene::nodes {

    class Model; class LightUnit; class RenderUnit;

    /// Represents the root node of a scene,
    /// managing models and global light units.
    class Scene final : public Group {
    public:
        /// Constructs a scene with the given name.
        /// @param name The name of the scene.
        explicit Scene(const std::string& name);

        /// Adds a model to the scene.
        /// @param model Shared pointer to the model to add.
        void addModel(const std::shared_ptr<Model>& model);

        /// Adds a global light unit to the scene.
        /// @param lightUnit Shared pointer to the light unit to add.
        void addLightUnit(const std::shared_ptr<LightUnit>& lightUnit);

        /// Gets a vector of all models in the scene.
        /// @return Reference to the vector of models.
        [[nodiscard]] const std::vector<std::shared_ptr<Model>>& getModels() const;

        /// Gets a vector of global light units in the scene.
        /// @return Reference to the vector of global light units.
        [[nodiscard]] const std::vector<std::shared_ptr<LightUnit>>& getGlobalLightUnits() const;

        /// Gets all light units in the scene, including global and model-specific ones.
        /// @return Vector containing all light units in the scene.
        [[nodiscard]] std::vector<std::shared_ptr<LightUnit>> getAllLightUnits() const;

    private:
        std::vector<std::shared_ptr<Model>> models_; ///< Models contained in the scene.
        std::vector<std::shared_ptr<LightUnit>> globalLightUnits_; ///< Global light units not part of any specific model.
    };

} // namespace pixelforge::scene::nodes
