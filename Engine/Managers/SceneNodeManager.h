#pragma once
#include "../Scene/SceneNodes/SceneNode.h"
#include <iostream>
#include <unordered_map>
#include <stdexcept>


/// Class for managing scene node instances and ensuring their uniqueness.
class SceneNodeManager final {
public:
    /// Computes the hash for a scene node instance.
    /// @param node The scene node instance to hash.
    /// @return Hash value for the scene node.
    static std::shared_ptr<SceneNode> Register(const std::shared_ptr<SceneNode>& node) {
        const auto name = node->GetName();
        if (GetNodeByName(name)) {
            throw std::runtime_error("SceneNode with name '" + name + "' already exists!");
        }
        nodeMap[name] = node;
        return node;
    }

    /// Retrieves a scene node by its name.
    /// @param name The name of the scene node to retrieve.
    /// @return Shared pointer to the scene node, or nullptr if not found.
    static std::shared_ptr<SceneNode> GetNodeByName(const std::string& name) {
        if (const auto node = nodeMap.find(name); node != nodeMap.end()) {
            return node->second;
        }
        return nullptr;
    }

    /// Sets the parent of a given scene node by name.
    /// @param node The node whose parent is being set.
    /// @param parentName The name of the desired parent node.
    /// @throws std::runtime_error If the node name matches the parent name.
    static void SetParent(const std::shared_ptr<SceneNode>& node, const std::string& parentName) {
        if (node->GetName() == parentName) {
            throw std::runtime_error("Cannot set node '" + parentName + "' as its own parent!");
        }
        const auto parent = GetNodeByName(parentName);
        if (!parent) {
            std::cerr << "Parent node '" << parentName << "' not found!" << std::endl;
            return;
        }
        parent->AddChild(node);
    }

private:
    inline static std::unordered_map<std::string, std::shared_ptr<SceneNode>> nodeMap; ///< Registry of scene nodes by name.
};
