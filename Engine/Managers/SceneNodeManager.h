#pragma once
#include "../Scene/SceneNodes/SceneNode.h"
#include <iostream>
using namespace std;


class SceneNodeManager final {
public:
    static shared_ptr<SceneNode> Register(const shared_ptr<SceneNode>& node) {
        const auto name = node->GetName();
        if (GetNodeByName(name)) {
            throw runtime_error("SceneNode with name '" + name + "' already exists!");
        }
        nodeMap[name] = node;
        return node;
    }

    static shared_ptr<SceneNode> GetNodeByName(const string& name) {
        if (const auto node = nodeMap.find(name); node != nodeMap.end()) {
            return node->second;
        }
        return nullptr;
    }

    static void SetParent(const shared_ptr<SceneNode>& node, const string& parentName) {
        if (node->GetName() == parentName) {
            throw runtime_error("Cannot set node '" + parentName + "' as its own parent!");
        }
        const auto parent = GetNodeByName(parentName);
        if (!parent) {
            cerr << "Parent node '" << parentName << "' not found!" << endl;
            return;
        }
        parent->AddChild(node);
    }

private:
    inline static unordered_map<string, shared_ptr<SceneNode>> nodeMap;
};
