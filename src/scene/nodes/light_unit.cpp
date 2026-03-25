#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/transform/translate.hpp>
#include "managers/mesh_manager.hpp"
#include "managers/light_manager.hpp"


namespace pixelforge::scene::nodes {

    using lighting::Light;
    using geometry::Mesh;
    using transform::Translate;
    using std::string;
    using std::shared_ptr;
    using std::make_shared;
    using glm::vec3;
    using glm::vec4;
    using glm::mat4;


    LightUnit::LightUnit(const string& name):
        TransformNode(name) {
    }


    LightUnit::LightUnit(const string& name, const shared_ptr<Light>& light):
        TransformNode(name) {
        setLight(light);
    }


    void LightUnit::setLight(const shared_ptr<Light>& light) {
        this->light_ = management::LightManager::getOrCreate(light);
        setDefaultMesh();
    }


    void LightUnit::setMesh(const shared_ptr<Mesh>& mesh) {
        if (mesh) {
            this->mesh_ = management::MeshManager::getOrCreate(mesh);
        } else {
            this->mesh_ = nullptr;
        }
    }


    vec3 LightUnit::getCurrentPosition() const {
        const auto position = vec3(getGlobalTransform().toMatrix() * vec4(light_->getPosition(), 1.0f));
        return position;
    }


    vec3 LightUnit::getCurrentDirection() const {
        return normalize(mat3_cast(getGlobalTransform().getRotation()) * light_->getDirection());
    }


    mat4 LightUnit::getModelMatrix() const {
        return (getGlobalTransform() * nodeScale_ * Translate(light_->getPosition())).toMatrix();
    }


    const shared_ptr<Light>& LightUnit::getLight() const {
        return light_;
    }


    shared_ptr<Mesh>& LightUnit::getMesh() {
        return mesh_;
    }


    // TODO: Orienting Cone and Arrow in the direction of the Light
    // TODO: Cone and Arrow do show weird faulty rendering (faulty normals?)
    void LightUnit::setDefaultMesh() {
        switch (light_->getType()) {
            case Light::DIRECTIONAL: {
                mesh_ = management::MeshManager::getOrCreate(make_shared<Mesh>("Arrow.obj")); break;
            }
            case Light::POSITIONAL: {
                mesh_ =management::MeshManager::getOrCreate(make_shared<Mesh>("Sphere.obj")); break;
            }
            case Light::SPOT: {
                mesh_ = management::MeshManager::getOrCreate(make_shared<Mesh>("Cone.obj")); break;
            }
            default: break;
        }
    }

} // namespace pixelforge::scene::nodes
