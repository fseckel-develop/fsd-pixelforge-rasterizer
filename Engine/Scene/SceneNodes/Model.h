#pragma once
#include "TransformNode.h"
class Light; class Mesh; class LightUnit; class RenderUnit;


/// Extension of the TransformNode class that represents a model in a
/// scene, hierarchically composable of render units and light units.
class Model final : public TransformNode {
public:
    /// Constructs a model with a given name.
    /// @param name The name of the model.
    explicit Model(const std::string& name);

    /// Adds a light unit to the model.
    /// @param lightUnit Shared pointer to the light unit to be added.
    void AddLightUnit(const std::shared_ptr<LightUnit>& lightUnit);

    /// Adds a render unit to the model.
    /// @param renderUnit Shared pointer to the render unit to be added.
    void AddRenderUnit(const std::shared_ptr<RenderUnit>& renderUnit);

    /// Retrieves a light unit by its name.
    /// @param name The name of the light unit to be retrieved.
    /// @return Shared pointer to the light unit with the matching name, or nullptr if not found.
    [[nodiscard]] std::shared_ptr<LightUnit> GetLightUnitByName(const std::string& name) const;

    /// Retrieves a render unit by its name.
    /// @param name The name of the render unit to be retrieved.
    /// @return Shared pointer to the render unit with the matching name, or nullptr if not found.
    [[nodiscard]] std::shared_ptr<RenderUnit> GetRenderUnitByName(const std::string& name) const;

    /// Retrieves all light units in the model.
    /// @return Reference to the vector of shared pointers to light units.
    [[nodiscard]] const std::vector<std::shared_ptr<LightUnit>>& GetLightUnits();

    /// Retrieves all render units in the model.
    /// @return Reference to the vector of shared pointers to render units.
    [[nodiscard]] const std::vector<std::shared_ptr<RenderUnit>>& GetRenderUnits();

private:
    std::vector<std::shared_ptr<LightUnit>> lightUnits;   ///< List of light units associated with the model.
    std::vector<std::shared_ptr<RenderUnit>> renderUnits; ///< List of render units associated with the model.
};
