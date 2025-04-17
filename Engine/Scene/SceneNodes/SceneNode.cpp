#include "SceneNode.h"
#include "../Animations/Animation.h"
#include <iostream>


SceneNode::SceneNode(const string& name) {
    this->id = nextID++;
    SetName(name);
}


void SceneNode::SetName(const string& name) {
    this->name = name.empty() ? GenerateUniqueName() : name;
}


void SceneNode::SetTransform(const Transform& transform) {
    this->localTransform = transform;
    this->initialTransform = transform;
}


void SceneNode::AddAnimation(Animation* animation) {
    if (animation) {
        animation->Reset();
        animations.push_back(animation);
    }
}


void SceneNode::AddChild(SceneNode* child) {
    if (!child or child == this) return;
    if (ranges::find(children, child) != children.end()) return;
    if (child->parent) {
        auto& siblings = child->parent->children;
        erase(siblings, child);
    }
    children.push_back(child);
    child->SetParent(this);
}


void SceneNode::RemoveChild(SceneNode* child) {
    if (!child) return;
    const auto it = ranges::find(children, child);
    if (it != children.end()) {
        children.erase(it);
        child->SetParent(nullptr);
    }
    else {
        cerr << "Child '" << child->GetName() << "' from parent '" << this->name << "' not found for removal." << endl;
    }
}


string SceneNode::GetName() const {
    return name;
}


const SceneNode* SceneNode::GetRoot() const {
    auto current = this;
    while (current->parent) {
        current = current->parent;
    }
    return current;
}



void SceneNode::Update(const float deltaTime) {
    Transform animationOffset;
    for (const auto animation : animations) {
        if (animation && animation->IsPlaying()) {
            animationOffset = animation->GetOffset(deltaTime) * animationOffset;
        }
    }
    localTransform = initialTransform * animationOffset;
}


void SceneNode::SetParent(SceneNode* parent) {
    this->parent = parent;
    ancestorCount = parent ? parent->ancestorCount + 1 : 0;
}


mat4 SceneNode::GetModelMatrix() const {
    mat4 modelMatrix = localTransform.ToMatrix();
    auto current = this;
    while (0 < current->ancestorCount) {
        current = current->parent;
        modelMatrix = current->localTransform.ToMatrix() * modelMatrix;
    }
    return modelMatrix;
}


string SceneNode::GenerateUniqueName() const {
    return "SceneNode_" + to_string(this->id);
}
