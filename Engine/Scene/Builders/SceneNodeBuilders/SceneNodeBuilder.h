#pragma once
#include "../../../Managers/SceneNodeManager.h"


/// Base class template for creating fluent-style builders for specific SceneNode types.
/// @tparam NodeT The specific SceneNode subclass to build.
/// @tparam DerivedBuilder The concrete builder class inheriting from this template.
template<typename NodeT, typename DerivedBuilder>
class SceneNodeBuilder {
public:
    /// Starts the building process for a scene node with a name and parent (optional).
    /// @param name The name of the scene node.
    /// @param parentName The name of the parent node (optional).
    explicit SceneNodeBuilder(const std::string& name, const std::string& parentName = "") {
        if (SceneNodeManager::GetNodeByName(name)) {
            throw std::runtime_error("SceneNode with name '" + name + "' already exists!");
        }
        const auto node = SceneNodeManager::Register(std::make_shared<NodeT>(name));
        sceneNode = std::static_pointer_cast<NodeT>(node);
        if (!parentName.empty()) SceneNodeManager::SetParent(sceneNode, parentName);
    }

    /// Sets the name of the scene node being built.
    /// @param newName The new name for the scene node.
    /// @return Reference to the derived builder for fluent chaining.
    auto& withName(const std::string& newName) {
        sceneNode->SetName(newName);
        return static_cast<DerivedBuilder&>(*this);
    }

    /// Finalizes the build process and returns the scene node.
    /// @return Shared pointer to the constructed scene node.
    std::shared_ptr<NodeT> Build() const {
        return sceneNode;
    }

    /// Implicit conversion to allow the builder to be used as a scene node.
    /// @return Shared pointer to the constructed scene node.
    operator std::shared_ptr<NodeT>() { // NOLINT
        return Build();
    }

protected:
    std::shared_ptr<NodeT> sceneNode; ///< The underlying scene node being built.
};
