#pragma once
#include "SceneNodeBuilder.h"


template<typename NodeT, typename DerivedBuilder>
class GroupBuilder : public SceneNodeBuilder<NodeT, DerivedBuilder> {
public:
    explicit GroupBuilder(const string& name, const string& parentName = ""):
        SceneNodeBuilder<NodeT, DerivedBuilder>(name, parentName) {
    }
};
