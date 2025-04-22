#pragma once
#include <vector>
class Model; class Light; class RenderUnit; class Mesh;
using namespace std;


class Scene {
public:
    Scene() = default;
    void AddModel(const shared_ptr<Model>&);
    void AddLight(const shared_ptr<Light>&);
    void AddRenderUnit(const shared_ptr<RenderUnit>&);
    [[nodiscard]] const vector<shared_ptr<Model>>& GetModels() const;
    [[nodiscard]] const vector<shared_ptr<Light>>& GetGlobalLights() const;
    [[nodiscard]] vector<shared_ptr<Light>> GetAllLights() const;
    void Update(float) const;
    void Clear();

private:
    vector<shared_ptr<Model>> models;
    vector<shared_ptr<Light>> globalLights;
};
