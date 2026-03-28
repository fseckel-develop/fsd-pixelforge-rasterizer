#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/transform/translate.hpp>
#include <pixelforge/geometry/meshes/sphere.hpp>
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
        setLight(std::make_shared<lighting::AmbientLight>());
    }


    LightUnit::LightUnit(const string& name, const shared_ptr<Light>& light):
        TransformNode(name) {
        setLight(light ? light : make_shared<lighting::AmbientLight>());
    }


    void LightUnit::setLight(const shared_ptr<Light>& light) {
        this->light_ = management::LightManager::getOrCreate(light);
        // setDefaultMesh();
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
        if (!light_) {
            return (getGlobalTransform() * nodeScale_).toMatrix();
        }
        switch (light_->getType()) {
            case Light::POSITIONAL:
            case Light::SPOT:
                return (getGlobalTransform() * nodeScale_ * Translate(light_->getPosition())).toMatrix();
            case Light::AMBIENT:
            case Light::DIRECTIONAL:
            default:
                return (getGlobalTransform() * nodeScale_).toMatrix();
        }
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
                mesh_ = management::MeshManager::getOrCreate(make_shared<geometry::Sphere>()); break;
            }
            case Light::SPOT: {
                mesh_ = management::MeshManager::getOrCreate(make_shared<Mesh>("Cone.obj")); break;
            }
            default: break;
        }
    }

} // namespace pixelforge::scene::nodes
