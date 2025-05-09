#pragma once
#include "TransformNode.h"
#include "../Lights/Light.h"


/// Extension of the TransformNode class that represents a light source in the scene,
/// managing light data and an optional mesh to visually represent the light source.
class LightUnit final : public TransformNode {
public:
    /// Constructs a light unit with a given name.
    /// @param name The name of the light unit.
    explicit LightUnit(const std::string& name);

    /// Constructs a light unit node with a given name and light.
    /// @param name The name of the light unit.
    /// @param light Shared pointer to the light to be used.
    LightUnit(const std::string& name, const std::shared_ptr<Light>& light);

    /// Sets the light for the light unit.
    /// @param light Shared pointer to the light to be used.
    void SetLight(const std::shared_ptr<Light>& light);

    /// Sets the mesh for the light unit.
    /// @param mesh Shared pointer to the mesh used in the light unit.
    void SetMesh(const std::shared_ptr<Mesh>& mesh);

    /// Gets the current position of the light.
    /// @return 3D vector representing the current world-space position of the light.
    [[nodiscard]] glm::vec3 GetCurrentPosition() const;

    /// Gets the current direction of the light.
    /// @return 3D vector representing the current direction of the light.
    [[nodiscard]] glm::vec3 GetCurrentDirection() const;

    /// Gets the model matrix of the light unit.
    /// @return The model matrix combining all effective transforms.
    [[nodiscard]] glm::mat4 GetModelMatrix() const override;

    /// Gets the light associated with the light unit.
    /// @return Reference to the shared pointer of the light.
    [[nodiscard]] const std::shared_ptr<Light>& GetLight() const;

    /// Gets the mesh associated with the light unit.
    /// @return Reference to the shared pointer of the mesh.
    [[nodiscard]] std::shared_ptr<Mesh>& GetMesh();

private:
    std::shared_ptr<Light> light; ///< The light associated with the light unit.
    std::shared_ptr<Mesh> mesh = nullptr; ///< The mesh representing the light source (optional).

    /// Sets a default mesh for the light unit based on the type of its associated light.
    void SetDefaultMesh();
};
