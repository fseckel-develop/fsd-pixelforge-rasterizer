#pragma once
#include "SceneNode.h"


/// Alias for the SceneNode class, acting as a container that can hold other nodes.
class Group : public SceneNode {
public:
    /// Constructs a group with a given name.
    /// @param name The name of the group.
    explicit Group(const std::string& name);
};
