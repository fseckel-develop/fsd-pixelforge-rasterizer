#pragma once
#include "TransformNode.h"
#include <unordered_map>
#include <string>
class Light; class RenderUnit;
using namespace std;


class Model final : public TransformNode {
public:
    explicit Model(const string&);
    void AddLight(Light*, const string& = "");
    void AddRenderUnit(RenderUnit*, const string& = "");
    template <typename T> shared_ptr<T> LastAddedAs() const;
    [[nodiscard]] shared_ptr<Light> GetLightByName(const string&) const;
    [[nodiscard]] shared_ptr<RenderUnit> GetRenderUnitByName(const string&) const;
    [[nodiscard]] vector<shared_ptr<Light>>& GetLights();
    [[nodiscard]] vector<shared_ptr<RenderUnit>>& GetRenderUnits();
    void UpdateSelf(float) override {}

private:
    shared_ptr<SceneNode> lastAdded = nullptr;
    vector<shared_ptr<Light>> lights;
    vector<shared_ptr<RenderUnit>> renderUnits;
    template <typename T> void AddSceneNode(const shared_ptr<T>&, const string&, vector<shared_ptr<T>>&);
    shared_ptr<SceneNode> FindSceneNodeByName(const string&) const;
};


template <typename T>
shared_ptr<T> Model::LastAddedAs() const {
    return dynamic_pointer_cast<T>(lastAdded);
}


template<typename T>
void Model::AddSceneNode(const shared_ptr<T>& node, const string& parentName, vector<shared_ptr<T>>& container) {
    if (!node) return;
    const string& nodeName = node->GetName();
    for (const auto& existing : container) {
        if (existing && existing->GetName() == nodeName) {
            throw runtime_error("Node '" + nodeName + "' already exists in model '" + this->name + "'.");
        }
    }
    container.push_back(node);
    if (!parentName.empty()) {
        auto parent = FindSceneNodeByName(parentName);
        if (!parent) {
            throw runtime_error("Parent '" + parentName + "' for node '" + nodeName + "' not found in model '" + this->name + "'.");
        }
        parent->AddChild(node);
    } else {
        AddChild(node);
    }
    lastAdded = node;
}


// inline Model SolarSystem() {
//     Model model;
//     model.AddRenderable("Sun", new Sphere()); {
//         model.LastAdded()->SetTexture("../Resources/Textures/sunMap.jpg");
//         model.LastAdded()->AddAnimation(new Rotation(vec3(0.0f, 1.0f, 0.0f), 360.0f, 16.0f));
//     }
//     model.AddRenderable("Earth", new Sphere()); {
//         model.LastAdded()->SetTexture("../Resources/Textures/earthMap.jpg");
//         model.LastAdded()->SetTransform(Scale(vec3(0.4f, 0.4f, 0.4f)));
//         model.LastAdded()->AddAnimation(new Rotation(vec3(0.0f, 1.0f, 0.0f), 360.0f, 8.0f));
//         model.LastAdded()->AddAnimation(new Orbiting(2.0f, vec3(0.0f, 1.0f, 0.0f), 360.0f, 14.0f));
//     }
//     model.AddRenderable("Shuttle", new Mesh("../resources/models/shuttle.obj"), "Earth"); {
//         model.LastAdded()->SetTexture("../resources/textures/shuttle.jpg");
//         model.LastAdded()->SetTransform(Transform(vec3(0.0f), vec3(0.7f, 1.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
//         model.LastAdded()->AddAnimation(new Orbiting(1.0f, vec3(0.5f, 1.0f, 0.0f), 360.0f, 8.0f));
//     }
//     return model;
// }
