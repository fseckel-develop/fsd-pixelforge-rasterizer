#include "SceneNode.h"
#include <ranges>


SceneNode::SceneNode(const string& name) {
    this->id = nextID++;
    SetName(name);
}


void SceneNode::SetName(const string& name) {
    this->name = name.empty() ? "SceneNode_" + to_string(this->id) : name;
}


void SceneNode::AddChild(const shared_ptr<SceneNode>& child) {
    if (!child or child.get() == this) return;
    if (ranges::find(children, child) != children.end()) return;
    if (child->IsAncestorOf(shared_from_this())) return;
    if (const auto currentParent = child->parent.lock()) {
        currentParent->RemoveChild(child);
    }
    child->SetParent(shared_from_this());
    children.push_back(child);
}


void SceneNode::RemoveChild(const shared_ptr<SceneNode>& child) {
    if (!child or child.get() == this) return;
    if (const auto it = ranges::remove(children, child).begin(); it != children.end()) {
        children.erase(it, children.end());
        child->parent.reset();
    }
}


void SceneNode::InsertNodeAbove(const shared_ptr<SceneNode>& node) {
    if (!node) return;
    if (const auto oldParent = GetParent()) {
        oldParent->RemoveChild(shared_from_this());
        oldParent->AddChild(node);
    }
    node->AddChild(shared_from_this());
}


int SceneNode::GetID() const {
    return id;
}


string SceneNode::GetName() const {
    return name;
}


shared_ptr<SceneNode> SceneNode::GetParent() const {
    return parent.lock();
}


shared_ptr<SceneNode> SceneNode::GetRoot() {
    auto current = shared_from_this();
    while (current->parent.lock()) {
        current = current->parent.lock();
    }
    return current;
}


vector<shared_ptr<SceneNode>> SceneNode::GetChildren() const {
    return children;
}


void SceneNode::UpdateSelfAndChildren(const float deltaTime) {
    stack<shared_ptr<SceneNode>> nodeStack;
    nodeStack.push(shared_from_this());
    while (!nodeStack.empty()) {
        const auto current = nodeStack.top();
        nodeStack.pop();
        current->UpdateSelf(deltaTime); // Optional hook
        for (auto& child : ranges::reverse_view(current->children)) {
            nodeStack.push(child);
        }
    }
}


void SceneNode::SetParent(const shared_ptr<SceneNode>& parent) {
    if (!parent or parent.get() == this) return;
    this->parent = parent;
    ancestorCount = parent ? parent->ancestorCount + 1 : 0;
}


bool SceneNode::IsAncestorOf(const shared_ptr<SceneNode>& node) const {
    if (!node) return false;
    if (this->ancestorCount >= node->ancestorCount) return false;
    auto current = node->parent.lock();
    while (current) {
        if (current.get() == this) return true;
        current = current->parent.lock();
    }
    return false;
}
