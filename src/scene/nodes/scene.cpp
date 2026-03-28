#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/graphics/texturing/texture.hpp>
#include <pixelforge/graphics/texturing/cube_map.hpp>
#include "managers/texture_manager.hpp"


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

    // TODO: Add instance management for cube maps
    void Scene::setCubeMap(const shared_ptr<graphics::CubeMap>& cubeMap) {
        cubeMap_ = cubeMap;
        skySphereTexture_ = nullptr;
    }


    void Scene::setCubeMap(const string& baseName) {
        setCubeMap(std::make_shared<graphics::CubeMap>(baseName));
    }


    void Scene::setSkySphereTexture(const std::shared_ptr<graphics::Texture>& texture) {
        skySphereTexture_ = management::TextureManager::getOrCreate(texture);
        cubeMap_ = nullptr;
    }


    void Scene::setSkySphereTexture(const std::string& fileName) {
        setSkySphereTexture(std::make_shared<graphics::Texture>(fileName));
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


    const shared_ptr<graphics::CubeMap>& Scene::getCubeMap() const {
        return cubeMap_;
    }


    bool Scene::hasCubeMap() const {
        return cubeMap_ != nullptr;
    }


    const std::shared_ptr<graphics::Texture>& Scene::getSkySphereTexture() const {
        return skySphereTexture_;
    }


    bool Scene::hasSkySphereTexture() const {
        return skySphereTexture_ != nullptr;
    }


} // namespace pixelforge::scene::nodes
