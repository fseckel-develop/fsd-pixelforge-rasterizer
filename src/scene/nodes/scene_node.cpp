#include <pixelforge/scene/nodes/scene_node.hpp>
#include <ranges>


namespace pixelforge::scene::nodes {

    using std::vector;
    using std::stack;
    using std::string;
    using std::to_string;
    using std::shared_ptr;
    using std::ranges::find;
    using std::ranges::remove;
    using std::ranges::reverse_view;


    SceneNode::SceneNode(const string& name) {
        this->id_ = nextID_++;
        setName(name);
    }


    void SceneNode::setName(const string& name) {
        this->name_ = name.empty() ? "SceneNode_" + to_string(this->id_) : name;
    }


    void SceneNode::addChild(const shared_ptr<SceneNode>& child) {
        if (!child or child.get() == this) return;
        if (find(children_, child) != children_.end()) return;
        if (child->isAncestorOf(shared_from_this())) return;
        if (const auto currentParent = child->parent_.lock()) {
            currentParent->removeChild(child);
        }
        child->setParent(shared_from_this());
        children_.push_back(child);
    }


    void SceneNode::removeChild(const shared_ptr<SceneNode>& child) {
        if (!child or child.get() == this) return;
        if (const auto it = remove(children_, child).begin(); it != children_.end()) {
            children_.erase(it, children_.end());
            child->parent_.reset();
        }
    }


    void SceneNode::insertNodeAbove(const shared_ptr<SceneNode>& node) {
        if (!node) return;
        if (const auto oldParent = getParent()) {
            oldParent->removeChild(shared_from_this());
            oldParent->addChild(node);
        }
        node->addChild(shared_from_this());
    }


    int SceneNode::getID() const {
        return id_;
    }


    string SceneNode::getName() const {
        return name_;
    }


    shared_ptr<SceneNode> SceneNode::getParent() const {
        return parent_.lock();
    }


    shared_ptr<SceneNode> SceneNode::getRoot() {
        auto current = shared_from_this();
        while (current->parent_.lock()) {
            current = current->parent_.lock();
        }
        return current;
    }


    vector<shared_ptr<SceneNode>> SceneNode::getChildren() const {
        return children_;
    }


    void SceneNode::updateSelfAndChildren(const float deltaTime) {
        stack<shared_ptr<SceneNode>> nodeStack;
        nodeStack.push(shared_from_this());
        while (!nodeStack.empty()) {
            const auto current = nodeStack.top();
            nodeStack.pop();
            current->updateSelf(deltaTime); // Optional hook
            for (auto& child : reverse_view(current->children_)) {
                nodeStack.push(child);
            }
        }
    }


    void SceneNode::setParent(const shared_ptr<SceneNode>& parent) {
        if (!parent or parent.get() == this) return;
        this->parent_ = parent;
        ancestorCount_ = parent ? parent->ancestorCount_ + 1 : 0;
    }


    bool SceneNode::isAncestorOf(const shared_ptr<SceneNode>& node) const {
        if (!node) return false;
        if (this->ancestorCount_ >= node->ancestorCount_) return false;
        auto current = node->parent_.lock();
        while (current) {
            if (current.get() == this) return true;
            current = current->parent_.lock();
        }
        return false;
    }

} // namespace pixelforge::scene::nodes
