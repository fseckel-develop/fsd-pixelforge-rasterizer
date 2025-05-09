#pragma once
#include <vector>
#include "Group.h"
class Model; class Light; class LightUnit; class RenderUnit;


/// Represents the root node of a scene,
/// managing models and global light units.
class Scene final : public Group {
public:
    /// Constructs a scene with the given name.
    /// @param name The name of the scene.
    explicit Scene(const std::string& name);

    /// Adds a model to the scene.
    /// @param model Shared pointer to the model to add.
    void AddModel(const std::shared_ptr<Model>& model);

    /// Adds a global light unit to the scene.
    /// @param lightUnit Shared pointer to the light unit to add.
    void AddLightUnit(const std::shared_ptr<LightUnit>& lightUnit);

    /// Gets a vector of all models in the scene.
    /// @return Reference to the vector of models.
    [[nodiscard]] const std::vector<std::shared_ptr<Model>>& GetModels() const;

    /// Gets a vector of global light units in the scene.
    /// @return Reference to the vector of global light units.
    [[nodiscard]] const std::vector<std::shared_ptr<LightUnit>>& GetGlobalLightUnits() const;

    /// Gets all light units in the scene, including global and model-specific ones.
    /// @return Vector containing all light units in the scene.
    [[nodiscard]] std::vector<std::shared_ptr<LightUnit>> GetAllLightUnits() const;

private:
    std::vector<std::shared_ptr<Model>> models; ///< Models contained in the scene.
    std::vector<std::shared_ptr<LightUnit>> globalLightUnits; ///< Global light units not part of any specific model.
};
