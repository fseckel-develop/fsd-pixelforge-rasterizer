#pragma once
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/scene_node.hpp>
#include <GLM/vec3.hpp>
#include <memory>
#include <string>


namespace pixelforge::scene::utilities {

    /// Prints the subtree rooted at the given scene node.
    /// @param node Root node of the subtree to print.
    void printSceneNodeTree(const std::shared_ptr<nodes::SceneNode>& node);

    /// Prints a compact summary of a scene, including model and light counts.
    /// @param scene The scene to summarize.
    void printSceneSummary(const std::shared_ptr<nodes::Scene>& scene);

    /// Validates and normalizes a direction vector.
    /// @param direction The direction vector to validate.
    /// @param context A string representing the context of the direction.
    /// @return The normalized direction vector.
    /// @throws std::invalid_argument If the direction vector is zero-length.
    glm::vec3 validateDirection(const glm::vec3& direction, const std::string& context);

} // namespace pixelforge::scene::utilities
