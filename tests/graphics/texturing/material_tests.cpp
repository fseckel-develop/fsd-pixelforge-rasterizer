#include <doctest.h>
#include <managers/material_manager.hpp>
#include <managers/texture_manager.hpp>
#include <pixelforge/graphics/texturing/material.hpp>
#include <pixelforge/graphics/texturing/texture.hpp>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <memory>
#include <string>

using pixelforge::graphics::BlackPlastic;
using pixelforge::graphics::Gold;
using pixelforge::graphics::Material;
using pixelforge::graphics::Texture;
using pixelforge::management::MaterialManager;
using pixelforge::management::TextureManager;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec4Approx(const glm::vec4& actual, const glm::vec4& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
        CHECK(actual.w == doctest::Approx(expected.w).epsilon(EPS));
    }

    struct MaterialManagersGuard {
        MaterialManagersGuard() {
            MaterialManager::clear();
            TextureManager::clear();
        }

        ~MaterialManagersGuard() {
            TextureManager::clear();
            MaterialManager::clear();
        }
    };

} // namespace


TEST_CASE("Material default constructor initializes empty maps and zero values") {
    MaterialManagersGuard guard;

    const Material material;

    CHECK(material.getDiffuseMap() == nullptr);
    CHECK(material.getSpecularMap() == nullptr);
    checkVec4Approx(material.getAmbient(), glm::vec4(0.0f));
    checkVec4Approx(material.getDiffuse(), glm::vec4(0.0f));
    checkVec4Approx(material.getSpecular(), glm::vec4(0.0f));
    CHECK(material.getShininess() == doctest::Approx(0.0f));
}

TEST_CASE("Material texture-path constructor creates diffuse map") {
    MaterialManagersGuard guard;
    TextureManager::initialize();

    const Material material("test.jpg");

    REQUIRE(material.getDiffuseMap() != nullptr);
    CHECK(material.getSpecularMap() == nullptr);
    CHECK(material.getDiffuseMap()->getFilePath().find("test.jpg") != std::string::npos);
}

TEST_CASE("Material setDiffuseMap stores managed diffuse texture") {
    MaterialManagersGuard guard;
    TextureManager::initialize();

    auto texture = std::make_shared<Texture>("test.jpg");
    Material material;

    material.setDiffuseMap(texture);

    REQUIRE(material.getDiffuseMap() != nullptr);
    CHECK(material.getDiffuseMap() == texture);
}

TEST_CASE("Material setSpecularMap stores managed specular texture") {
    MaterialManagersGuard guard;
    TextureManager::initialize();

    auto texture = std::make_shared<Texture>("test.jpg");
    Material material;

    material.setSpecularMap(texture);

    REQUIRE(material.getSpecularMap() != nullptr);
    CHECK(material.getSpecularMap() == texture);
}

TEST_CASE("Material setters update color components and shininess") {
    MaterialManagersGuard guard;

    Material material;
    material.setAmbientColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    material.setDiffuseColor(glm::vec4(0.4f, 0.5f, 0.6f, 1.0f));
    material.setSpecularColor(glm::vec4(0.7f, 0.8f, 0.9f, 1.0f));
    material.setShininess(64.0f);

    checkVec4Approx(material.getAmbient(), glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    checkVec4Approx(material.getDiffuse(), glm::vec4(0.4f, 0.5f, 0.6f, 1.0f));
    checkVec4Approx(material.getSpecular(), glm::vec4(0.7f, 0.8f, 0.9f, 1.0f));
    CHECK(material.getShininess() == doctest::Approx(64.0f));
}

TEST_CASE("Material unbindTextures releases bound diffuse and specular texture units") {
    MaterialManagersGuard guard;
    TextureManager::initialize();

    auto diffuse = std::make_shared<Texture>("test.jpg");
    auto specular = std::make_shared<Texture>("test.jpg");

    Material material;
    material.setDiffuseMap(diffuse);
    material.setSpecularMap(specular);

    static_cast<void>(material.getDiffuseMap()->bindTexture());
    static_cast<void>(material.getSpecularMap()->bindTexture());

    REQUIRE(material.getDiffuseMap()->getUnit() != TextureManager::noUnit());
    REQUIRE(material.getSpecularMap()->getUnit() != TextureManager::noUnit());

    material.unbindTextures();

    CHECK(material.getDiffuseMap()->getUnit() == TextureManager::noUnit());
    CHECK(material.getSpecularMap()->getUnit() == TextureManager::noUnit());
}

TEST_CASE("Material unbindTextures is safe when maps are missing") {
    MaterialManagersGuard guard;

    Material material;

    glGetError();
    material.unbindTextures();
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Gold material preset has expected representative values") {
    MaterialManagersGuard guard;

    const Gold material;

    checkVec4Approx(material.getAmbient(), glm::vec4(0.2473f, 0.1995f, 0.0745f, 1.0000f));
    checkVec4Approx(material.getDiffuse(), glm::vec4(0.7516f, 0.6065f, 0.2265f, 1.0000f));
    checkVec4Approx(material.getSpecular(), glm::vec4(0.6283f, 0.5558f, 0.3661f, 1.0000f));
    CHECK(material.getShininess() == doctest::Approx(51.200f));
}

TEST_CASE("BlackPlastic material preset has expected representative values") {
    MaterialManagersGuard guard;

    const BlackPlastic material;

    checkVec4Approx(material.getAmbient(), glm::vec4(0.0000f, 0.0000f, 0.0000f, 1.0000f));
    checkVec4Approx(material.getDiffuse(), glm::vec4(0.0100f, 0.0100f, 0.0100f, 1.0000f));
    checkVec4Approx(material.getSpecular(), glm::vec4(0.5000f, 0.5000f, 0.5000f, 1.0000f));
    CHECK(material.getShininess() == doctest::Approx(32.000f));
}
