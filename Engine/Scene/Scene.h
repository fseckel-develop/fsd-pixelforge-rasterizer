#pragma once
#include <vector>
class Model; class Light; class Renderable;
using namespace std;


class Scene {
public:
    Scene() = default;
    void AddModel(Model*);
    void AddLight(Light*);
    void AddRenderable(Renderable*);
    [[nodiscard]] vector<Light*> GetAllLights() const;
    void Update(float) const;
    void Clear();

private:
    vector<Model*> models;
    vector<Light*> globalLights;
    vector<Renderable*> globalRenderables;
};
