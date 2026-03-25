#pragma once
#include <pixelforge/scene/nodes/scene_node.hpp>
#include <iostream>
#include <unordered_map>
#include <stdexcept>


namespace pixelforge::management {

    /// Class for managing scene node instances and ensuring their uniqueness.
    class SceneNodeManager final {
    public:
        /// Registers a scene node instance.
        /// @param node The scene node instance to register.
        /// @return Hash value for the scene node.
        static std::shared_ptr<scene::nodes::SceneNode> registerNode(const std::shared_ptr<scene::nodes::SceneNode>& node) {
            const auto name = node->getName();
            if (getNodeByName(name)) {
                throw std::runtime_error("SceneNode with name '" + name + "' already exists! Different name needed!");
            }
            nodeMap_[name] = node;
            return node;
        }

        /// Retrieves a scene node by its name.
        /// @param name The name of the scene node to retrieve.
        /// @return Shared pointer to the scene node, or nullptr if not found.
        static std::shared_ptr<scene::nodes::SceneNode> getNodeByName(const std::string& name) {
            if (const auto node = nodeMap_.find(name); node != nodeMap_.end()) {
                return node->second;
            }
            return nullptr;
        }


        /// Sets the parent of a given scene node by name.
        /// @param node The node whose parent is being set.
        /// @param parentName The name of the desired parent node.
        /// @throws std::runtime_error If the node name matches the parent name.
        static void setParent(const std::shared_ptr<scene::nodes::SceneNode>& node, const std::string& parentName) {
            if (node->getName() == parentName) {
                throw std::runtime_error("Cannot set node '" + parentName + "' as its own parent!");
            }
            const auto parent = getNodeByName(parentName);
            if (!parent) {
                throw std::runtime_error("Parent node '" + parentName + "' not found!");
            }
            parent->addChild(node);
        }

        /// Clears the complete scene node registry.
        static void clear() {
            nodeMap_.clear();
        }

    private:
        inline static std::unordered_map<std::string, std::shared_ptr<scene::nodes::SceneNode>> nodeMap_; ///< Registry of scene nodes by name.
    };

} // namespace pixelforge::management
