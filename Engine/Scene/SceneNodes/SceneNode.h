#pragma once
#include <ostream>
#include <vector>


/// Abstract base class for a node in the scene graph,
/// enabling a one-parent-multiple-children hierarchy.
class SceneNode : public std::enable_shared_from_this<SceneNode> {
public:
    /// Constructs a scene node with a given name.
    /// @param name The name of the node.
    explicit SceneNode(const std::string& name);

    /// Sets the name of the node.
    /// @param name The name to set for this node.
    void SetName(const std::string& name);

    /// Adds a child node to this node.
    /// @param child The child node to add.
    void AddChild(const std::shared_ptr<SceneNode>& child);

    /// Removes a child node from this node.
    /// @param child The child node to remove.
    void RemoveChild(const std::shared_ptr<SceneNode>& child);

    /// Inserts this node above another node in the scene graph hierarchy.
    /// @param node The node to insert above the current node.
    void InsertNodeAbove(const std::shared_ptr<SceneNode>& node);

    /// Gets the ID of this node.
    /// @return The unique ID of the node.
    [[nodiscard]] int GetID() const;

    /// Gets the name of this node.
    /// @return The name of the node.
    [[nodiscard]] std::string GetName() const;

    /// Gets the parent of this node.
    /// @return Shared pointer to the parent node, or an expired weak pointer if no parent exists.
    [[nodiscard]] std::shared_ptr<SceneNode> GetParent() const;

    /// Gets the root node of the scene graph this node belongs to.
    /// @return Shared pointer to the root node.
    [[nodiscard]] std::shared_ptr<SceneNode> GetRoot();

    /// Gets the children of this node.
    /// @return Vector of shared pointers to the child nodes.
    [[nodiscard]] std::vector<std::shared_ptr<SceneNode>> GetChildren() const;

    /// Updates this node and all of its children.
    /// @param deltaTime The time passed since the last update.
    void UpdateSelfAndChildren(float deltaTime);

    /// Virtual destructor for cleanup.
    virtual ~SceneNode() = default;

protected:
    int id; ///< Unique identifier for the node.
    std::string name; ///< Name of the node.
    int ancestorCount = 0; ///< The number of ancestors of this node in the scene graph.
    std::weak_ptr<SceneNode> parent; ///< Weak pointer to the parent node.
    std::vector<std::shared_ptr<SceneNode>> children; ///< List of child nodes.

    /// Updates this node. To be overridden.
    virtual void UpdateSelf(float /*deltaTime*/) {}

private:
    inline static int nextID = 0; ///< Static counter for generating unique node IDs.

    /// Sets the parent of this node. Internally called when adding/removing a child.
    /// @param parent Shared pointer to the new parent node.
    virtual void SetParent(const std::shared_ptr<SceneNode>& parent);

    /// Checks if this node is an ancestor of the given node.
    /// @param node Shared pointer to the node to check.
    /// @return True if this node is an ancestor of the given node, false otherwise.
    bool IsAncestorOf(const std::shared_ptr<SceneNode>& node) const;
};
