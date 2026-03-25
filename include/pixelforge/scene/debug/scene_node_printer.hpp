#pragma once
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/scene_node.hpp>
#include <memory>


namespace pixelforge::scene::debug {

    /// Prints the subtree rooted at the given scene node.
    /// @param node Root node of the subtree to print.
    void printSceneNodeTree(const std::shared_ptr<nodes::SceneNode>& node);

    /// Prints a compact summary of a scene, including model and light counts.
    /// @param scene The scene to summarize.
    void printSceneSummary(const std::shared_ptr<nodes::Scene>& scene);

} // namespace pixelforge::scene::debug
