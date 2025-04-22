#include "SceneNode.h"
#include "../Animations/Animation.h"
#include <iostream>


SceneNode::SceneNode(const string& name) {
    this->id = nextID++;
    SetName(name);
}


void SceneNode::SetName(const string& name) {
    this->name = name.empty() ? "SceneNode_" + to_string(this->id) : name;
}


void SceneNode::SetParent(const shared_ptr<SceneNode>& parent) {
    if (const auto currentParent = this->parent.lock()) {
        currentParent->RemoveChild(shared_from_this());
    }
    if (parent) {
        parent->AddChild(shared_from_this());
    }
    this->parent = parent;
    ancestorCount = parent ? parent->ancestorCount + 1 : 0;
}


void SceneNode::SetTransform(const Transform& transform) {
    this->localTransform = transform;
    this->initialTransform = transform;
    MarkTransformDirty();
}


void SceneNode::SetNodeScale(const vec3& scale) {
    this->scale = Scale(scale);
}



void SceneNode::AddAnimation(Animation* animation) {
    if (animation) {
        animation->Reset();
        animations.push_back(shared_ptr<Animation>(animation));
    }
}


string SceneNode::GetName() const {
    return name;
}


mat4 SceneNode::GetModelMatrix() const {
    if (transformDirty) {
        modelMatrix = localTransform.ToMatrix();
        auto current = shared_from_this();
        while (current->ancestorCount > 0) {
            current = current->parent.lock();
            modelMatrix = current->localTransform.ToMatrix() * modelMatrix;
        }
        transformDirty = false;
    }
    return modelMatrix * scale.ToMatrix();
}


void SceneNode::Update(const float deltaTime) {
    Transform animationOffset;
    for (const auto& animation : animations) {
        if (animation && animation->IsPlaying()) {
            animationOffset = animation->GetOffset(deltaTime) * animationOffset;
        }
    }
    localTransform = initialTransform * animationOffset;
    MarkTransformDirty();
}


void SceneNode::AddChild(const shared_ptr<SceneNode>& child) {
    if (child.get() == this) return;
    if (ranges::find(children, child) != children.end()) return;
    if (const auto currentParent = child->parent.lock()) {
        currentParent->RemoveChild(child);
    }
    children.push_back(child);
}


void SceneNode::RemoveChild(const shared_ptr<SceneNode>& child) {
    const auto it = ranges::find(children, child); // NOLINT
    if (it != children.end()) {
        children.erase(it);
        child->parent.reset();
    } else {
        cerr << "Child '" << child->GetName() << "' from parent '" << this->name << "' not found for removal." << endl;
    }
}


void SceneNode::MarkTransformDirty() {
    stack<shared_ptr<SceneNode>> nodeStack;
    nodeStack.push(shared_from_this());
    while (!nodeStack.empty()) {
        const auto node = nodeStack.top();
        nodeStack.pop();
        if (!node->transformDirty) {
            node->transformDirty = true;
            for (const auto& child : node->children) {
                nodeStack.push(child);
            }
        }
    }
}
