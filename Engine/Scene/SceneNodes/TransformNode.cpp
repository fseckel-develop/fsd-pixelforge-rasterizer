#include "TransformNode.h"
#include "AnimationNode.h"
#include "../Animations/Animation.h"
#include <queue>
#include <ranges>


TransformNode::TransformNode(const string& name):
    SceneNode(name) {
}


void TransformNode::SetTransform(const Transform& transform) {
    localTransform = transform;
    MarkGlobalTransformDirty();
}


void TransformNode::SetNodeScale(const vec3& scale) {
    nodeScale = Scale(scale);
}


void TransformNode::MarkGlobalTransformDirty() {
    queue<std::shared_ptr<SceneNode>> nodesToVisit;
    nodesToVisit.push(shared_from_this());
    while (!nodesToVisit.empty()) {
        auto currentNode = nodesToVisit.front();
        nodesToVisit.pop();
        for (const auto& child : currentNode->GetChildren()) {
            nodesToVisit.push(child);
        }
        if (const auto transformNode = std::dynamic_pointer_cast<TransformNode>(currentNode)) {
            transformNode->globalTransformDirty = true;
        }
    }
}


void TransformNode::AddAnimation(const shared_ptr<Animation>& animation) {
    const auto currentParent = GetParent();
    auto animationNode = dynamic_pointer_cast<AnimationNode>(currentParent);
    if (!animationNode) {
        animationNode = make_shared<AnimationNode>(GetName() + "-Animation");
        InsertNodeAbove(animationNode);
    }
    animationNode->AddAnimation(animation);
}


bool TransformNode::IsGlobalTransformDirty() const {
    return globalTransformDirty;
}


const Transform& TransformNode::GetGlobalTransform() const { // NOLINT
    if (globalTransformDirty) UpdateGlobalTransform();
    return globalTransform;
}


mat4 TransformNode::GetModelMatrix() const {
    return (GetGlobalTransform() * nodeScale).ToMatrix();
}


void TransformNode::UpdateSelf(const float /*deltaTime*/) {
    if (globalTransformDirty) {
        UpdateGlobalTransform();
    }
}


shared_ptr<TransformNode> TransformNode::FindNextTransformAncestorFrom(const shared_ptr<SceneNode>& node) {
    auto current = node->GetParent();
    while (current) {
        if (auto transformNode = std::dynamic_pointer_cast<TransformNode>(current)) {
            return transformNode;
        }
        current = current->GetParent();
    }
    return nullptr;
}


void TransformNode::UpdateGlobalTransform() const { // NOLINT
    vector<Transform> transformChain;
    shared_ptr<SceneNode> current = const_cast<TransformNode*>(this)->shared_from_this();
    Transform baseTransform;
    while (current) {
        if (const auto transformNode = dynamic_pointer_cast<TransformNode>(current)) {
            if (!transformNode->IsGlobalTransformDirty()) {
                baseTransform = transformNode->GetGlobalTransform();
                break;
            }
            transformChain.push_back(transformNode->localTransform);
        }
        current = FindNextTransformAncestorFrom(current);
    }
    globalTransform = baseTransform;
    for (auto& transform : ranges::reverse_view(transformChain)) {
        globalTransform = globalTransform * transform;
    }
    globalTransformDirty = false;
}
