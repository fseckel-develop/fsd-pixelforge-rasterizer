#include "Material.h"
#include "../../Managers/TextureManager.h"


Material::Material() {
    this->diffuseMap = nullptr;
    this->specularMap = nullptr;
}


Material::Material(const string& path) {
    this->diffuseMap = TextureManager::GetOrCreate(make_shared<Texture>(path));
    this->specularMap = nullptr;
}


Material::Material(const shared_ptr<Texture>& diffuseMap, const shared_ptr<Texture>& specularMap, const vec4 ambient, const vec4 diffuse, const vec4 specular, const float shininess) {
    this->diffuseMap = TextureManager::GetOrCreate(diffuseMap);
    this->specularMap = TextureManager::GetOrCreate(specularMap);
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}


void Material::SetDiffuseMap(const shared_ptr<Texture>& diffuseMap) {
    this->diffuseMap = TextureManager::GetOrCreate(diffuseMap);
}


void Material::SetSpecularMap(const shared_ptr<Texture>& specularMap) {
    this->specularMap = TextureManager::GetOrCreate(specularMap);
}


void Material::SetAmbientColor(const vec4 ambientColor) {
    this->ambient = ambientColor;
}


void Material::SetDiffuseColor(const vec4 diffuseColor) {
    this->diffuse = diffuseColor;
}


void Material::SetSpecularColor(const vec4 specularColor) {
    this->specular = specularColor;
}


void Material::SetShininess(const float shininess) {
    this->shininess = shininess;
}


shared_ptr<Texture> Material::GetDiffuseMap() const {
    return this->diffuseMap;
}


shared_ptr<Texture> Material::GetSpecularMap() const {
    return this->specularMap;
}


vec4 Material::GetAmbient() const {
    return this->ambient;
}


vec4 Material::GetDiffuse() const {
    return this->diffuse;
}


vec4 Material::GetSpecular() const {
    return this->specular;
}


float Material::GetShininess() const {
    return this->shininess;
}


void Material::UnbindTextures() const {
    if (diffuseMap) diffuseMap->UnbindTexture();
    if (specularMap) specularMap->UnbindTexture();
}


// Values found in:
// http://devernay.free.fr/cours/opengl/materials.html
// Computer Graphics Programming in OpenGL with C++ [Gordon, Clevenger]

Gold::Gold() {
    this->ambient = vec4(0.2473f, 0.1995f, 0.0745f, 1.0000f);
    this->diffuse = vec4(0.7516f, 0.6065f, 0.2265f, 1.0000f);
    this->specular = vec4(0.6283f, 0.5558f, 0.3661f, 1.0000f);
    this->shininess = 51.200f;
}


Brass::Brass() {
    this->ambient = vec4(0.3294f, 0.2235f, 0.0275f, 1.0000f);
    this->diffuse = vec4(0.7804f, 0.5686f, 0.1137f, 1.0000f);
    this->specular = vec4(0.9922f, 0.9412f, 0.8078f, 1.0000f);
    this->shininess = 27.897f;
}


Chrome::Chrome() {
    this->ambient = vec4(0.2500f, 0.2500f, 0.2500f, 1.0000f);
    this->diffuse = vec4(0.4000f, 0.4000f, 0.4000f, 1.0000f);
    this->specular = vec4(0.7746f, 0.7746f, 0.7746f, 1.0000f);
    this->shininess = 76.800f;
}


Copper::Copper() {
    this->ambient = vec4(0.1913f, 0.0735f, 0.0225f, 1.0000f);
    this->diffuse = vec4(0.7038f, 0.2705f, 0.0828f, 1.0000f);
    this->specular = vec4(0.2568f, 0.1376f, 0.0860f, 1.0000f);
    this->shininess = 12.800f;
}


Silver::Silver() {
    this->ambient = vec4(0.1923f, 0.1923f, 0.1923f, 1.0000f);
    this->diffuse = vec4(0.5075f, 0.5075f, 0.5075f, 1.0000f);
    this->specular = vec4(0.5083f, 0.5083f, 0.5083f, 1.0000f);
    this->shininess = 51.200f;
}


Bronze::Bronze() {
    this->ambient = vec4(0.2125f, 0.1275f, 0.0540f, 1.0000f);
    this->diffuse = vec4(0.7140f, 0.4284f, 0.1814f, 1.0000f);
    this->specular = vec4(0.3935f, 0.2719f, 0.1667f, 1.0000f);
    this->shininess = 25.600f;
}


::Ruby::Ruby() {
    this->ambient = vec4(0.1745f, 0.0118f, 0.0118f, 0.9500f);
    this->diffuse = vec4(0.6142f, 0.0414f, 0.0414f, 0.9500f);
    this->specular = vec4(0.7278f, 0.6270f, 0.6270f, 0.9500f);
    this->shininess = 76.800f;
}


Jade::Jade() {
    this->ambient = vec4(0.1350f, 0.2225f, 0.1575f, 0.9500f);
    this->diffuse = vec4(0.5400f, 0.8900f, 0.6300f, 0.9500f);
    this->specular = vec4(0.3162f, 0.3162f, 0.3162f, 0.9500f);
    this->shininess = 12.800f;
}


Pearl::Pearl() {
    this->ambient = vec4(0.2500f, 0.2073f, 0.2073f, 0.9220f);
    this->diffuse = vec4(1.0000f, 0.8290f, 0.8290f, 0.9220f);
    this->specular = vec4(0.2966f, 0.2966f, 0.2966f, 0.9220f);
    this->shininess = 11.264f;
}


Emerald::Emerald() {
    this->ambient = vec4(0.0215f, 0.1745f, 0.0215f, 0.9500f);
    this->diffuse = vec4(0.0757f, 0.6142f, 0.0757f, 0.9500f);
    this->specular = vec4(0.6330f, 0.7278f, 0.6330f, 0.9500f);
    this->shininess = 78.800f;
}


Obsidian::Obsidian() {
    this->ambient = vec4(0.0538f, 0.0500f, 0.0663f, 1.0000f);
    this->diffuse = vec4(0.1828f, 0.1700f, 0.2253f, 1.0000f);
    this->specular = vec4(0.3327f, 0.3286f, 0.3464f, 1.0000f);
    this->shininess = 38.400f;
}


Turquoise::Turquoise() {
    this->ambient = vec4(0.1000f, 0.1873f, 0.1745f, 0.9500f);
    this->diffuse = vec4(0.3960f, 0.7415f, 0.6910f, 0.9500f);
    this->specular = vec4(0.2972f, 0.3083f, 0.3067f, 0.9500f);
    this->shininess = 12.800f;
}


RedPlastic::RedPlastic() {
    this->ambient = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.5000f, 0.0000f, 0.0000f, 1.0000f);
    this->specular = vec4(0.7000f, 0.6000f, 0.6000f, 1.0000f);
    this->shininess = 32.000f;
}


CyanPlastic::CyanPlastic() {
    this->ambient = vec4(0.0000f, 0.1000f, 0.0600f, 1.0000f);
    this->diffuse = vec4(0.0000f, 0.5098f, 0.5098f, 1.0000f);
    this->specular = vec4(0.5020f, 0.5020f, 0.5020f, 1.0000f);
    this->shininess = 32.000f;
}


GreenPlastic::GreenPlastic() {
    this->ambient = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.1000f, 0.3500f, 0.1000f, 1.0000f);
    this->specular = vec4(0.4500f, 0.5500f, 0.4500f, 1.0000f);
    this->shininess = 32.000f;
}


YellowPlastic::YellowPlastic() {
    this->ambient = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.5000f, 0.5000f, 0.0000f, 1.0000f);
    this->specular = vec4(0.6000f, 0.6000f, 0.5000f, 1.0000f);
    this->shininess = 32.000f;
}


BlackPlastic::BlackPlastic() {
    this->ambient = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.0100f, 0.0100f, 0.0100f, 1.0000f);
    this->specular = vec4(0.5000f, 0.5000f, 0.5000f, 1.0000f);
    this->shininess = 32.000f;
}


WhitePlastic::WhitePlastic() {
    this->ambient = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.5500f, 0.5500f, 0.5500f, 1.0000f);
    this->specular = vec4(0.7000f, 0.7000f, 0.7000f, 1.0000f);
    this->shininess = 32.000f;
}


RedRubber::RedRubber() {
    this->ambient = vec4(0.0500f, 0.0000f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.5000f, 0.4000f, 0.4000f, 1.0000f);
    this->specular = vec4(0.7000f, 0.0400f, 0.0400f, 1.0000f);
    this->shininess = 10.000f;
}


CyanRubber::CyanRubber() {
    this->ambient = vec4(0.0000f, 0.0500f, 0.0500f, 1.0000f);
    this->diffuse = vec4(0.4000f, 0.5000f, 0.5000f, 1.0000f);
    this->specular = vec4(0.0400f, 0.7000f, 0.7000f, 1.0000f);
    this->shininess = 10.000f;
}


GreenRubber::GreenRubber() {
    this->ambient = vec4(0.0000f, 0.0500f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.4000f, 0.5000f, 0.4000f, 1.0000f);
    this->specular = vec4(0.0400f, 0.7000f, 0.0400f, 1.0000f);
    this->shininess = 10.000f;
}


YellowRubber::YellowRubber() {
    this->ambient = vec4(0.0500f, 0.0500f, 0.0000f, 1.0000f);
    this->diffuse = vec4(0.5000f, 0.5000f, 0.4000f, 1.0000f);
    this->specular = vec4(0.7000f, 0.7000f, 0.0400f, 1.0000f);
    this->shininess = 10.000f;
}


BlackRubber::BlackRubber() {
    this->ambient = vec4(0.0200f, 0.0200f, 0.0200f, 1.0000f);
    this->diffuse = vec4(0.0100f, 0.0100f, 0.0100f, 1.0000f);
    this->specular = vec4(0.4000f, 0.4000f, 0.4000f, 1.0000f);
    this->shininess = 10.000f;
}


WhiteRubber::WhiteRubber() {
    this->ambient = vec4(0.0500f, 0.0500f, 0.0500f, 1.0000f);
    this->diffuse = vec4(0.5000f, 0.5000f, 0.5000f, 1.0000f);
    this->specular = vec4(0.7000f, 0.7000f, 0.7000f, 1.0000f);
    this->shininess = 10.000f;
}
