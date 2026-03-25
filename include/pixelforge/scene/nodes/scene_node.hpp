#pragma once
#include <ostream>
#include <vector>


namespace pixelforge::scene::nodes {

    /// Abstract base class for a node in the scene graph,
    /// enabling a one-parent-multiple-children hierarchy.
    class SceneNode : public std::enable_shared_from_this<SceneNode> {
    public:
        /// Constructs a scene node with a given name.
        /// @param name The name of the node.
        explicit SceneNode(const std::string& name);

        /// Sets the name of the node.
        /// @param name The name to set for this node.
        void setName(const std::string& name);

        /// Adds a child node to this node.
        /// @param child The child node to add.
        void addChild(const std::shared_ptr<SceneNode>& child);

        /// Removes a child node from this node.
        /// @param child The child node to remove.
        void removeChild(const std::shared_ptr<SceneNode>& child);

        /// Inserts this node above another node in the scene graph hierarchy.
        /// @param node The node to insert above the current node.
        void insertNodeAbove(const std::shared_ptr<SceneNode>& node);

        /// Gets the ID of this node.
        /// @return The unique ID of the node.
        [[nodiscard]] int getID() const;

        /// Gets the name of this node.
        /// @return The name of the node.
        [[nodiscard]] std::string getName() const;

        /// Gets the parent of this node.
        /// @return Shared pointer to the parent node, or an expired weak pointer if no parent exists.
        [[nodiscard]] std::shared_ptr<SceneNode> getParent() const;

        /// Gets the root node of the scene graph this node belongs to.
        /// @return Shared pointer to the root node.
        [[nodiscard]] std::shared_ptr<SceneNode> getRoot();

        /// Gets the children of this node.
        /// @return Vector of shared pointers to the child nodes.
        [[nodiscard]] std::vector<std::shared_ptr<SceneNode>> getChildren() const;

        /// Updates this node and all of its children.
        /// @param deltaTime The time passed since the last update.
        void updateSelfAndChildren(float deltaTime);

        /// Virtual destructor for cleanup.
        virtual ~SceneNode() = default;

    protected:
        int id_; ///< Unique identifier for the node.
        std::string name_; ///< Name of the node.
        int ancestorCount_ = 0; ///< The number of ancestors of this node in the scene graph.
        std::weak_ptr<SceneNode> parent_; ///< Weak pointer to the parent node.
        std::vector<std::shared_ptr<SceneNode>> children_; ///< List of child nodes.

        /// Updates this node. To be overridden.
        virtual void updateSelf(float /*deltaTime*/) {}

    private:
        inline static int nextID_ = 0; ///< Static counter for generating unique node IDs.

        /// Sets the parent of this node. Internally called when adding/removing a child.
        /// @param parent Shared pointer to the new parent node.
        virtual void setParent(const std::shared_ptr<SceneNode>& parent);

        /// Checks if this node is an ancestor of the given node.
        /// @param node Shared pointer to the node to check.
        /// @return True if this node is an ancestor of the given node, false otherwise.
        bool isAncestorOf(const std::shared_ptr<SceneNode>& node) const;
    };

} // namespace pixelforge::scene::nodes
