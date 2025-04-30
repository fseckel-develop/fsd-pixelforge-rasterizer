#pragma once
#include "TransformNode.h"
#include <iostream>
#include <string>
class Light; class Mesh; class LightUnit; class RenderUnit;
using namespace std;


class Model final : public TransformNode {
public:
    explicit Model(const string&);
    void AddLightUnit(const shared_ptr<LightUnit>&);
    void AddRenderUnit(const shared_ptr<RenderUnit>&);
    [[nodiscard]] shared_ptr<LightUnit> GetLightUnitByName(const string&) const;
    [[nodiscard]] shared_ptr<RenderUnit> GetRenderUnitByName(const string&) const;
    [[nodiscard]] const vector<shared_ptr<LightUnit>>& GetLightUnits();
    [[nodiscard]] const vector<shared_ptr<RenderUnit>>& GetRenderUnits();
    void UpdateSelf(float) override {}

private:
    vector<shared_ptr<LightUnit>> lightUnits;
    vector<shared_ptr<RenderUnit>> renderUnits;
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
