#include "RenderUnit.h"
#include "../../Managers/MaterialManager.h"
#include "../../Managers/MeshManager.h"


RenderUnit::RenderUnit(const string& name):
    TransformNode(name) {
}


RenderUnit::RenderUnit(const string& name, const shared_ptr<Mesh>& mesh):
    TransformNode(name),
    mesh(MeshManager::GetOrCreate(mesh)) {
}


void RenderUnit::SetMesh(const shared_ptr<Mesh>& mesh) {
    this->mesh = MeshManager::GetOrCreate(mesh);
}


void RenderUnit::SetMaterial(const shared_ptr<Material>& material) {
    this->material = MaterialManager::GetOrCreate(material);
}


// TODO: Differentiate between diffuse and specular Texture
void RenderUnit::SetTexture(const string& textureFileName) {
    this->SetMaterial(make_shared<Material>(textureFileName));
}


shared_ptr<Mesh>& RenderUnit::GetMesh() {
    return mesh;
}


shared_ptr<Material>& RenderUnit::GetMaterial() {
    return material;
}
