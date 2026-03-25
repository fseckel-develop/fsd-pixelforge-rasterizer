#include <pixelforge/graphics/texturing/material.hpp>
#include "managers/texture_manager.hpp"


namespace pixelforge::graphics {

    using std::string;
    using std::make_shared;
    using std::shared_ptr;
    using glm::vec4;


    Material::Material():
        ambient_(vec4(0.0f)),
        diffuse_(vec4(0.0f)),
        specular_(vec4(0.0f)),
        shininess_(0.0f) {
    }


    Material::Material(const string& path):
        ambient_(vec4(0.0f)),
        diffuse_(vec4(0.0f)),
        specular_(vec4(0.0f)),
        shininess_(0.0f) {
        this->diffuseMap_ = management::TextureManager::getOrCreate(make_shared<Texture>(path));
    }


    void Material::setDiffuseMap(const shared_ptr<Texture>& diffuseMap) {
        this->diffuseMap_ = management::TextureManager::getOrCreate(diffuseMap);
    }


    void Material::setSpecularMap(const shared_ptr<Texture>& specularMap) {
        this->specularMap_ = management::TextureManager::getOrCreate(specularMap);
    }


    void Material::setAmbientColor(const vec4& ambientColor) {
        this->ambient_ = ambientColor;
    }


    void Material::setDiffuseColor(const vec4& diffuseColor) {
        this->diffuse_ = diffuseColor;
    }


    void Material::setSpecularColor(const vec4& specularColor) {
        this->specular_ = specularColor;
    }


    void Material::setShininess(const float shininess) {
        this->shininess_ = shininess;
    }


    const shared_ptr<Texture>& Material::getDiffuseMap() const {
        return this->diffuseMap_;
    }


    const shared_ptr<Texture>& Material::getSpecularMap() const {
        return this->specularMap_;
    }


    const vec4& Material::getAmbient() const {
        return this->ambient_;
    }


    const vec4& Material::getDiffuse() const {
        return this->diffuse_;
    }


    const vec4& Material::getSpecular() const {
        return this->specular_;
    }


    float Material::getShininess() const {
        return this->shininess_;
    }


    void Material::unbindTextures() const {
        if (diffuseMap_) diffuseMap_->unbindTexture();
        if (specularMap_) specularMap_->unbindTexture();
    }


    // Values found in:
    // http://devernay.free.fr/cours/opengl/materials.html
    // Computer Graphics Programming in OpenGL with C++ [Gordon, Clevenger]

    Gold::Gold() {
        this->ambient_ = vec4(0.2473f, 0.1995f, 0.0745f, 1.0000f);
        this->diffuse_ = vec4(0.7516f, 0.6065f, 0.2265f, 1.0000f);
        this->specular_ = vec4(0.6283f, 0.5558f, 0.3661f, 1.0000f);
        this->shininess_ = 51.200f;
    }


    Brass::Brass() {
        this->ambient_ = vec4(0.3294f, 0.2235f, 0.0275f, 1.0000f);
        this->diffuse_ = vec4(0.7804f, 0.5686f, 0.1137f, 1.0000f);
        this->specular_ = vec4(0.9922f, 0.9412f, 0.8078f, 1.0000f);
        this->shininess_ = 27.897f;
    }


    Chrome::Chrome() {
        this->ambient_ = vec4(0.2500f, 0.2500f, 0.2500f, 1.0000f);
        this->diffuse_ = vec4(0.4000f, 0.4000f, 0.4000f, 1.0000f);
        this->specular_ = vec4(0.7746f, 0.7746f, 0.7746f, 1.0000f);
        this->shininess_ = 76.800f;
    }


    Copper::Copper() {
        this->ambient_ = vec4(0.1913f, 0.0735f, 0.0225f, 1.0000f);
        this->diffuse_ = vec4(0.7038f, 0.2705f, 0.0828f, 1.0000f);
        this->specular_ = vec4(0.2568f, 0.1376f, 0.0860f, 1.0000f);
        this->shininess_ = 12.800f;
    }


    Silver::Silver() {
        this->ambient_ = vec4(0.1923f, 0.1923f, 0.1923f, 1.0000f);
        this->diffuse_ = vec4(0.5075f, 0.5075f, 0.5075f, 1.0000f);
        this->specular_ = vec4(0.5083f, 0.5083f, 0.5083f, 1.0000f);
        this->shininess_ = 51.200f;
    }


    Bronze::Bronze() {
        this->ambient_ = vec4(0.2125f, 0.1275f, 0.0540f, 1.0000f);
        this->diffuse_ = vec4(0.7140f, 0.4284f, 0.1814f, 1.0000f);
        this->specular_ = vec4(0.3935f, 0.2719f, 0.1667f, 1.0000f);
        this->shininess_ = 25.600f;
    }


    Ruby::Ruby() {
        this->ambient_ = vec4(0.1745f, 0.0118f, 0.0118f, 0.9500f);
        this->diffuse_ = vec4(0.6142f, 0.0414f, 0.0414f, 0.9500f);
        this->specular_ = vec4(0.7278f, 0.6270f, 0.6270f, 0.9500f);
        this->shininess_ = 76.800f;
    }


    Jade::Jade() {
        this->ambient_ = vec4(0.1350f, 0.2225f, 0.1575f, 0.9500f);
        this->diffuse_ = vec4(0.5400f, 0.8900f, 0.6300f, 0.9500f);
        this->specular_ = vec4(0.3162f, 0.3162f, 0.3162f, 0.9500f);
        this->shininess_ = 12.800f;
    }


    Pearl::Pearl() {
        this->ambient_ = vec4(0.2500f, 0.2073f, 0.2073f, 0.9220f);
        this->diffuse_ = vec4(1.0000f, 0.8290f, 0.8290f, 0.9220f);
        this->specular_ = vec4(0.2966f, 0.2966f, 0.2966f, 0.9220f);
        this->shininess_ = 11.264f;
    }


    Emerald::Emerald() {
        this->ambient_ = vec4(0.0215f, 0.1745f, 0.0215f, 0.9500f);
        this->diffuse_ = vec4(0.0757f, 0.6142f, 0.0757f, 0.9500f);
        this->specular_ = vec4(0.6330f, 0.7278f, 0.6330f, 0.9500f);
        this->shininess_ = 78.800f;
    }


    Obsidian::Obsidian() {
        this->ambient_ = vec4(0.0538f, 0.0500f, 0.0663f, 1.0000f);
        this->diffuse_ = vec4(0.1828f, 0.1700f, 0.2253f, 1.0000f);
        this->specular_ = vec4(0.3327f, 0.3286f, 0.3464f, 1.0000f);
        this->shininess_ = 38.400f;
    }


    Turquoise::Turquoise() {
        this->ambient_ = vec4(0.1000f, 0.1873f, 0.1745f, 0.9500f);
        this->diffuse_ = vec4(0.3960f, 0.7415f, 0.6910f, 0.9500f);
        this->specular_ = vec4(0.2972f, 0.3083f, 0.3067f, 0.9500f);
        this->shininess_ = 12.800f;
    }


    RedPlastic::RedPlastic() {
        this->ambient_ = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.5000f, 0.0000f, 0.0000f, 1.0000f);
        this->specular_ = vec4(0.7000f, 0.6000f, 0.6000f, 1.0000f);
        this->shininess_ = 32.000f;
    }


    CyanPlastic::CyanPlastic() {
        this->ambient_ = vec4(0.0000f, 0.1000f, 0.0600f, 1.0000f);
        this->diffuse_ = vec4(0.0000f, 0.5098f, 0.5098f, 1.0000f);
        this->specular_ = vec4(0.5020f, 0.5020f, 0.5020f, 1.0000f);
        this->shininess_ = 32.000f;
    }


    GreenPlastic::GreenPlastic() {
        this->ambient_ = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.1000f, 0.3500f, 0.1000f, 1.0000f);
        this->specular_ = vec4(0.4500f, 0.5500f, 0.4500f, 1.0000f);
        this->shininess_ = 32.000f;
    }


    YellowPlastic::YellowPlastic() {
        this->ambient_ = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.5000f, 0.5000f, 0.0000f, 1.0000f);
        this->specular_ = vec4(0.6000f, 0.6000f, 0.5000f, 1.0000f);
        this->shininess_ = 32.000f;
    }


    BlackPlastic::BlackPlastic() {
        this->ambient_ = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.0100f, 0.0100f, 0.0100f, 1.0000f);
        this->specular_ = vec4(0.5000f, 0.5000f, 0.5000f, 1.0000f);
        this->shininess_ = 32.000f;
    }


    WhitePlastic::WhitePlastic() {
        this->ambient_ = vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.5500f, 0.5500f, 0.5500f, 1.0000f);
        this->specular_ = vec4(0.7000f, 0.7000f, 0.7000f, 1.0000f);
        this->shininess_ = 32.000f;
    }


    RedRubber::RedRubber() {
        this->ambient_ = vec4(0.0500f, 0.0000f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.5000f, 0.4000f, 0.4000f, 1.0000f);
        this->specular_ = vec4(0.7000f, 0.0400f, 0.0400f, 1.0000f);
        this->shininess_ = 10.000f;
    }


    CyanRubber::CyanRubber() {
        this->ambient_ = vec4(0.0000f, 0.0500f, 0.0500f, 1.0000f);
        this->diffuse_ = vec4(0.4000f, 0.5000f, 0.5000f, 1.0000f);
        this->specular_ = vec4(0.0400f, 0.7000f, 0.7000f, 1.0000f);
        this->shininess_ = 10.000f;
    }


    GreenRubber::GreenRubber() {
        this->ambient_ = vec4(0.0000f, 0.0500f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.4000f, 0.5000f, 0.4000f, 1.0000f);
        this->specular_ = vec4(0.0400f, 0.7000f, 0.0400f, 1.0000f);
        this->shininess_ = 10.000f;
    }


    YellowRubber::YellowRubber() {
        this->ambient_ = vec4(0.0500f, 0.0500f, 0.0000f, 1.0000f);
        this->diffuse_ = vec4(0.5000f, 0.5000f, 0.4000f, 1.0000f);
        this->specular_ = vec4(0.7000f, 0.7000f, 0.0400f, 1.0000f);
        this->shininess_ = 10.000f;
    }


    BlackRubber::BlackRubber() {
        this->ambient_ = vec4(0.0200f, 0.0200f, 0.0200f, 1.0000f);
        this->diffuse_ = vec4(0.0100f, 0.0100f, 0.0100f, 1.0000f);
        this->specular_ = vec4(0.4000f, 0.4000f, 0.4000f, 1.0000f);
        this->shininess_ = 10.000f;
    }


    WhiteRubber::WhiteRubber() {
        this->ambient_ = vec4(0.0500f, 0.0500f, 0.0500f, 1.0000f);
        this->diffuse_ = vec4(0.5000f, 0.5000f, 0.5000f, 1.0000f);
        this->specular_ = vec4(0.7000f, 0.7000f, 0.7000f, 1.0000f);
        this->shininess_ = 10.000f;
    }

} // namespace pixelforge::graphics
