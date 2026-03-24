#pragma once
#include <pixelforge/scene/nodes/transform_node.hpp>
#include <pixelforge/scene/lighting/light.hpp>

namespace pixelforge::geometry { class Mesh; }

namespace pixelforge::scene::nodes {

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
        LightUnit(const std::string& name, const std::shared_ptr<lighting::Light>& light);

        /// Sets the light for the light unit.
        /// @param light Shared pointer to the light to be used.
        void setLight(const std::shared_ptr<lighting::Light>& light);

        /// Sets the mesh for the light unit.
        /// @param mesh Shared pointer to the mesh used in the light unit.
        void setMesh(const std::shared_ptr<geometry::Mesh>& mesh);

        /// Gets the current position of the light.
        /// @return 3D vector representing the current world-space position of the light.
        [[nodiscard]] glm::vec3 getCurrentPosition() const;

        /// Gets the current direction of the light.
        /// @return 3D vector representing the current direction of the light.
        [[nodiscard]] glm::vec3 getCurrentDirection() const;

        /// Gets the model matrix of the light unit.
        /// @return The model matrix combining all effective transforms.
        [[nodiscard]] glm::mat4 getModelMatrix() const override;

        /// Gets the light associated with the light unit.
        /// @return Reference to the shared pointer of the light.
        [[nodiscard]] const std::shared_ptr<lighting::Light>& getLight() const;

        /// Gets the mesh associated with the light unit.
        /// @return Reference to the shared pointer of the mesh.
        [[nodiscard]] std::shared_ptr<geometry::Mesh>& getMesh();

    private:
        std::shared_ptr<lighting::Light> light_; ///< The light associated with the light unit.
        std::shared_ptr<geometry::Mesh> mesh_ = nullptr; ///< The mesh representing the light source (optional).

        /// Sets a default mesh for the light unit based on the type of its associated light.
        void setDefaultMesh();
    };

} // namespace pixelforge::scene::nodes
