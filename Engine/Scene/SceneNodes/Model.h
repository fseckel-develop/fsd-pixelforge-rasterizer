#pragma once
#include "SceneNode.h"
#include <unordered_map>
class Light; class Renderable; class Mesh;
using namespace std;


class Model final : public SceneNode {
public:
    explicit Model(const string&);
    void AddLight(Light*, const string& = "");
    void AddRenderable(Renderable*, const string& = "");
    void AddRenderable(const string&, Mesh*, const string& = "");
    [[nodiscard]] Light* GetLight(const string&) const;
    [[nodiscard]] Renderable* GetRenderable(const string&) const;
    [[nodiscard]] unordered_map<string, Light*>& GetLights();
    [[nodiscard]] unordered_map<string, Renderable*>& GetRenderables();
    [[nodiscard]] SceneNode* LastAdded() const;
    void Update(float) override;

private:
    unordered_map<string, Renderable*> renderables;
    unordered_map<string, Light*> lights;
    SceneNode* lastAdded = nullptr;

    template <typename T> void AddSceneNode(T*, const string&, unordered_map<string, T*>&);
};




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
