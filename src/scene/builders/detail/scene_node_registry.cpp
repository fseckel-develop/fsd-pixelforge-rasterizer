#include <pixelforge/scene/builders/detail/scene_node_registry.hpp>
#include "managers/scene_node_manager.hpp"


namespace pixelforge::scene::builders::detail {

    bool hasSceneNode(const std::string& name) {
        return management::SceneNodeManager::getNodeByName(name) != nullptr;
    }

    std::shared_ptr<nodes::SceneNode> registerSceneNode(const std::shared_ptr<nodes::SceneNode>& node) {
        return management::SceneNodeManager::registerNode(node);
    }

    void setParent(const std::shared_ptr<nodes::SceneNode>& node, const std::string& parentName) {
        management::SceneNodeManager::setParent(node, parentName);
    }

} // namespace pixelforge::scene::builders::detail
