#pragma once
#include <pixelforge/scene/nodes/group.hpp>
#include <vector>

namespace  pixelforge::graphics { class Texture; class CubeMap; }

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
        /// @param baseName Base name of the cube map file set.
        void setCubeMap(const std::string& baseName);

        /// Assigns a texture to the scene for optional sky sphere rendering.
        /// @param texture Shared pointer to the texture. May be nullptr to clear it.
        void setSkySphereTexture(const std::shared_ptr<graphics::Texture>& texture);

        /// Assigns a texture to the scene for optional sky sphere rendering.
        /// @param fileName File name of the texture.
        void setSkySphereTexture(const std::string& fileName);

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

        /// Gets the scene texture used for optional sky sphere rendering.
        /// @return Shared pointer to the assigned cube map, or nullptr if none is set.
        [[nodiscard]] const std::shared_ptr<graphics::Texture>& getSkySphereTexture() const;

        /// Checks whether the scene currently has a sky sphere texture assigned.
        /// @return True if a sky sphere texture is assigned, false otherwise.
        [[nodiscard]] bool hasSkySphereTexture() const;

    private:
        std::vector<std::shared_ptr<Model>> models_; ///< Models contained in the scene.
        std::vector<std::shared_ptr<LightUnit>> globalLightUnits_; ///< Global light units not part of any specific model.
        std::shared_ptr<graphics::CubeMap> cubeMap_ = nullptr; ///< Optional scene-wide cube map for skybox/environment rendering.
        std::shared_ptr<graphics::Texture> skySphereTexture_ = nullptr; ///< Optional texture for sky sphere rendering.
    };

} // namespace pixelforge::scene::nodes
