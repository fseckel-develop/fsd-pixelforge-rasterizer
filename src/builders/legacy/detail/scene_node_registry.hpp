#pragma once
#include <memory>
#include <string>

namespace pixelforge::scene::nodes { class SceneNode; }

namespace pixelforge::scene::builders::detail {

    /// Checks whether a scene node with the given name already exists.
    /// @param name The name to check.
    /// @return True if a node with the given name exists, false otherwise.
    bool hasSceneNode(const std::string& name);

    /// Registers a scene node and returns the stored node instance.
    /// @param node The scene node to register.
    /// @return The registered scene node.
    std::shared_ptr<nodes::SceneNode> registerSceneNode(
        const std::shared_ptr<nodes::SceneNode>& node
    );

    /// Sets the parent of a scene node by parent name.
    /// @param node The scene node whose parent is to be set.
    /// @param parentName The name of the parent node.
    void setParent(
        const std::shared_ptr<nodes::SceneNode>& node,
        const std::string& parentName
    );

} // namespace pixelforge::scene::builders::detail
