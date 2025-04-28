#pragma once
#include "../../Managers/SceneNodeManager.h"


template<typename NodeT, typename DerivedBuilder> class SceneNodeBuilder {
public:
    explicit SceneNodeBuilder(const string& name, const string& parentName = "") {
        if (SceneNodeManager::GetNodeByName(name)) {
            throw runtime_error("SceneNode with name '" + name + "' already exists!");
        }
        const auto node = SceneNodeManager::Register(make_shared<NodeT>(name));
        sceneNode = static_pointer_cast<NodeT>(node);
        if (!parentName.empty()) {
            SceneNodeManager::SetParent(sceneNode, parentName);
        }
    }

    auto& withName(const std::string& newName) {
        sceneNode->SetName(newName);
        return static_cast<DerivedBuilder&>(*this);
    }

    shared_ptr<NodeT> Build() const {
        return sceneNode;
    }

    operator shared_ptr<NodeT>() { // NOLINT
        return Build();
    }

protected:
    shared_ptr<NodeT> sceneNode;
};
