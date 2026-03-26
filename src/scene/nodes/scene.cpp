#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>


namespace pixelforge::scene::nodes {

    using std::vector;
    using std::string;
    using std::shared_ptr;


    Scene::Scene(const string& name):
        Group(name) {
    }


    void Scene::addModel(const shared_ptr<Model>& model) {
        if (!model) return;
        models_.push_back(model);
        addChild(model->getRoot());
    }


    void Scene::addLightUnit(const shared_ptr<LightUnit>& lightUnit) {
        if (!lightUnit) return;
        globalLightUnits_.push_back(lightUnit);
        addChild(lightUnit->getRoot());
    }


    const vector<shared_ptr<Model>>& Scene::getModels() const {
        return models_;
    }


    const vector<shared_ptr<LightUnit>>& Scene::getGlobalLightUnits() const {
        return globalLightUnits_;
    }


    vector<shared_ptr<LightUnit>> Scene::getAllLightUnits() const {
        vector<shared_ptr<LightUnit>> allLights = getGlobalLightUnits();
        for (const auto& model : models_) {
            for (const auto& lightUnit : model->getLightUnits()) allLights.push_back(lightUnit);
        }
        return allLights;
    }

} // namespace pixelforge::scene::nodes
