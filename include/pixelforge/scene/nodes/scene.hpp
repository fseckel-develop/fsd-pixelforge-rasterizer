#pragma once
#include <pixelforge/scene/nodes/group.hpp>
#include <vector>

namespace  pixelforge::graphics { class CubeMap; }

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

        /// Assigns a cube map to the scene for optional skybox/environment rendering.
        /// @param cubeMap Shared pointer to the cube map. May be nullptr to clear it.
        void setCubeMap(const std::shared_ptr<graphics::CubeMap>& cubeMap);

        /// Assigns a cube map to the scene for optional skybox/environment rendering.
        /// @param baseName base name of the cube map file set.
        void setCubeMap(const std::string& baseName);

        /// Gets a vector of all models in the scene.
        /// @return Reference to the vector of models.
        [[nodiscard]] const std::vector<std::shared_ptr<Model>>& getModels() const;

        /// Gets a vector of global light units in the scene.
        /// @return Reference to the vector of global light units.
        [[nodiscard]] const std::vector<std::shared_ptr<LightUnit>>& getGlobalLightUnits() const;

        /// Gets all light units in the scene, including global and model-specific ones.
        /// @return Vector containing all light units in the scene.
        [[nodiscard]] std::vector<std::shared_ptr<LightUnit>> getAllLightUnits() const;

        /// Gets the scene cube map used for optional skybox/environment rendering.
        /// @return Shared pointer to the assigned cube map, or nullptr if none is set.
        [[nodiscard]] const std::shared_ptr<graphics::CubeMap>& getCubeMap() const;

        /// Checks whether the scene currently has a cube map assigned.
        /// @return True if a cube map is assigned, false otherwise.
        [[nodiscard]] bool hasCubeMap() const;

    private:
        std::vector<std::shared_ptr<Model>> models_; ///< Models contained in the scene.
        std::vector<std::shared_ptr<LightUnit>> globalLightUnits_; ///< Global light units not part of any specific model.
        std::shared_ptr<graphics::CubeMap> cubeMap_ = nullptr; ///< Optional scene-wide cube map for skybox/environment rendering.
    };

} // namespace pixelforge::scene::nodes
