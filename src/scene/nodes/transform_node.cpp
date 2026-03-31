#include <pixelforge/scene/nodes/transform_node.hpp>
#include <pixelforge/scene/nodes/animation_node.hpp>
#include <ranges>
#include <queue>
#include "managers/scene_node_manager.hpp"


namespace pixelforge::scene::nodes {

    using transform::Transform;
    using transform::Scale;
    using animation::Animation;
    using std::string;
    using std::queue;
    using std::vector;
    using std::shared_ptr;
    using std::ranges::reverse_view;
    using glm::vec3;
    using glm::mat4;


    TransformNode::TransformNode(const string& name):
        SceneNode(name),
        nodeScale_(Scale()) {
    }


    void TransformNode::setTransform(const Transform& transform) {
        localTransform_ = transform;
        markGlobalTransformDirty();
    }


    void TransformNode::setNodeScale(const float scale) {
        nodeScale_ = Scale(vec3(scale));
    }


    void TransformNode::setNodeScale(const vec3& scale) {
        nodeScale_ = Scale(scale);
    }


    void TransformNode::markGlobalTransformDirty() {
        queue<std::shared_ptr<SceneNode>> nodesToVisit;
        nodesToVisit.push(shared_from_this());
        while (!nodesToVisit.empty()) {
            auto currentNode = nodesToVisit.front();
            nodesToVisit.pop();
            for (const auto& child : currentNode->getChildren()) {
                nodesToVisit.push(child);
            }
            if (const auto transformNode = std::dynamic_pointer_cast<TransformNode>(currentNode)) {
                transformNode->globalTransformDirty_ = true;
            }
        }
    }


    void TransformNode::addAnimation(const shared_ptr<Animation>& animation) {
        const auto currentParent = getParent();
        auto animationNode = dynamic_pointer_cast<AnimationNode>(currentParent);
        if (!animationNode) {
            animationNode = make_shared<AnimationNode>(getName() + "-Animation");
            insertNodeAbove(management::SceneNodeManager::registerNode(animationNode));
        }
        animationNode->addAnimation(animation);
    }


    bool TransformNode::isGlobalTransformDirty() const {
        return globalTransformDirty_;
    }


    const Transform& TransformNode::getGlobalTransform() const { // NOLINT
        if (globalTransformDirty_) updateGlobalTransform();
        return globalTransform_;
    }


    mat4 TransformNode::getModelMatrix() const {
        return (getGlobalTransform() * nodeScale_).toMatrix();
    }


    void TransformNode::updateSelf(const float /*timeDelta*/) {
        if (globalTransformDirty_) {
            updateGlobalTransform();
        }
    }


    Transform TransformNode::getLocalTransform() const {
        return localTransform_;
    }


    shared_ptr<TransformNode> TransformNode::findNextTransformAncestorFrom(const shared_ptr<SceneNode>& node) {
        auto current = node->getParent();
        while (current) {
            if (auto transformNode = std::dynamic_pointer_cast<TransformNode>(current)) {
                return transformNode;
            }
            current = current->getParent();
        }
        return nullptr;
    }


    void TransformNode::updateGlobalTransform() const { // NOLINT
        std::vector<Transform> transformChain;
        std::shared_ptr<SceneNode> current = const_cast<TransformNode*>(this)->shared_from_this();
        Transform baseTransform;
        while (current) {
            if (const auto transformNode = std::dynamic_pointer_cast<TransformNode>(current)) {
                if (!transformNode->isGlobalTransformDirty()) {
                    baseTransform = transformNode->getGlobalTransform();
                    break;
                }
                Transform effectiveLocal = transformNode->getLocalTransform();
                const auto parent = transformNode->getParent();
                if (const auto animationNode = std::dynamic_pointer_cast<AnimationNode>(parent)) {
                    effectiveLocal = effectiveLocal * std::static_pointer_cast<TransformNode>(animationNode)->getLocalTransform();
                    transformChain.push_back(effectiveLocal);
                    current = findNextTransformAncestorFrom(animationNode);
                    continue;
                }
                transformChain.push_back(effectiveLocal);
            }
            current = findNextTransformAncestorFrom(current);
        }
        globalTransform_ = baseTransform;
        for (auto& transform : reverse_view(transformChain)) {
            globalTransform_ = globalTransform_ * transform;
        }
        globalTransformDirty_ = false;
    }

} // namespace pixelforge::scene::nodes
