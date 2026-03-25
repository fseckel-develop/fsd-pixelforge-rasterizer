#pragma once
#include "scene_node_builder.hpp"


namespace pixelforge::scene::nodes {

    /// Alias of the SceneNodeBuilder class for
    /// building a Group using a fluent interface.
    /// @tparam NodeT The specific Group subclass to build.
    /// @tparam DerivedBuilder The concrete builder class inheriting from this template.
    template<typename NodeT, typename DerivedBuilder>
    class GroupBuilder : public SceneNodeBuilder<NodeT, DerivedBuilder> {
    public:
        /// Starts the building process for a group with a name and parent (optional).
        /// @param name The name of the group.
        /// @param parentName The name of the parent node (optional).
        explicit GroupBuilder(const std::string& name, const std::string& parentName = ""):
            SceneNodeBuilder<NodeT, DerivedBuilder>(name, parentName) {
        }
    };

} // namespace pixelforge::scene::nodes
