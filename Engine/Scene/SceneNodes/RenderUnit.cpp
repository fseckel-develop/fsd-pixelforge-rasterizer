#include "RenderUnit.h"
#include "../../Geometry/Meshes/Mesh.h"
#include "../../Graphics/Texturing/Material.h"



RenderUnit::RenderUnit(const string& name, Mesh* mesh):
    SceneNode(name),
    mesh(shared_ptr<Mesh>(mesh)) {
}


void RenderUnit::SetMaterial(Material* material) {
    this->material = shared_ptr<Material>(material);
}


// TODO: Differentiate between diffuse and specular Texture
void RenderUnit::SetTexture(const string& filePath) {
    const auto texture = new Texture(filePath);
    const auto material = new Material();
    material->SetDiffuseMap(texture);
    material->SetSpecularMap(texture);
    this->SetMaterial(material);
}


shared_ptr<Mesh>& RenderUnit::GetMesh() {
    return mesh;
}


shared_ptr<Material>& RenderUnit::GetMaterial() {
    return material;
}
