#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/managers/mesh_manager.hpp>
#include <pixelforge/managers/material_manager.hpp>


namespace pixelforge::scene::nodes {

    using geometry::Mesh;
    using graphics::Material;
    using std::string;
    using std::shared_ptr;


    RenderUnit::RenderUnit(const string& name):
        TransformNode(name) {
    }


    RenderUnit::RenderUnit(const string& name, const shared_ptr<Mesh>& mesh):
        TransformNode(name),
        mesh_(management::MeshManager::getOrCreate(mesh)) {
    }


    void RenderUnit::setMesh(const shared_ptr<Mesh>& mesh) {
        this->mesh_ = management::MeshManager::getOrCreate(mesh);
    }


    void RenderUnit::setMaterial(const shared_ptr<Material>& material) {
        this->material_ = management::MaterialManager::getOrCreate(material);
    }


    // TODO: Differentiate between diffuse and specular Texture
    void RenderUnit::setTexture(const string& textureFileName) {
        this->setMaterial(make_shared<Material>(textureFileName));
    }


    const shared_ptr<Mesh>& RenderUnit::getMesh() const {
        return mesh_;
    }


    const shared_ptr<Material>& RenderUnit::getMaterial() const {
        return material_;
    }

} // namespace pixelforge::scene::nodes
