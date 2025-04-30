#pragma once
#include <vector>
#include "Group.h"
class Model; class Light; class LightUnit; class RenderUnit;
using namespace std;


class Scene final : public Group {
public:
    explicit Scene(const string&);
    void AddModel(const shared_ptr<Model>&);
    void AddLightUnit(const shared_ptr<LightUnit>&);
    [[nodiscard]] const vector<shared_ptr<Model>>& GetModels() const;
    [[nodiscard]] const vector<shared_ptr<LightUnit>>& GetGlobalLightUnits() const;
    [[nodiscard]] vector<shared_ptr<LightUnit>> GetAllLightUnits() const;


private:
    vector<shared_ptr<Model>> models;
    vector<shared_ptr<LightUnit>> globalLightUnits;
};
