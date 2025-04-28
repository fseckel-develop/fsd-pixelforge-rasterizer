#pragma once
#include <vector>
#include "Group.h"
class Model; class Light; class LightUnit; class RenderUnit;
using namespace std;


class Scene final : public Group {
public:
    explicit Scene(const string&);
    void AddModel(const shared_ptr<Model>&) const;
    void AddLight(const string&, const shared_ptr<Light>&) const;
    [[nodiscard]] vector<shared_ptr<Model>> GetModels() const;
    [[nodiscard]] vector<shared_ptr<LightUnit>> GetGlobalLights() const;
    [[nodiscard]] vector<shared_ptr<LightUnit>> GetAllLights() const;
    void UpdateSelf(float) override;

private:
    shared_ptr<Group> models = make_shared<Group>("Models");
    shared_ptr<Group> globalLights = make_shared<Group>("GlobalLights");
};
