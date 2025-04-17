#include "Renderable.h"
#include "../../Geometry/Meshes/Mesh.h"
#include "../../Graphics/Pipeline/ShaderProgram.h"


Renderable::Renderable(const string& name, Mesh* mesh):
    SceneNode(name),
    mesh(mesh) {
}


void Renderable::SetMaterial(const Material& material) {
    this->material = material;
}


// TODO: Differentiate between diffuse and specular Texture
void Renderable::SetTexture(const string& filePath) {
    const auto texture = new Texture(filePath);
    auto material = Material();
    material.SetDiffuseMap(texture);
    material.SetSpecularMap(texture);
    this->SetMaterial(material);
}


void Renderable::Render(ShaderProgram& program, const Transform& globalTransform) const {
    program.SetUniform("modelMatrix", globalTransform.ToMatrix() * GetModelMatrix());
    // program.SetMaterial(material);
    program.UseProgram();
    mesh->Render();
    material.UnbindTextures();
}
