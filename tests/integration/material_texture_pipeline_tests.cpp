#include <doctest.h>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/graphics/texturing/material.hpp>
#include <pixelforge/graphics/texturing/texture.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <managers/animation_manager.hpp>
#include <managers/curve_manager.hpp>
#include <managers/light_manager.hpp>
#include <managers/material_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <managers/scene_node_manager.hpp>
#include <managers/texture_manager.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <string>

using pixelforge::geometry::Cuboid;
using pixelforge::graphics::Material;
using pixelforge::graphics::Texture;
using pixelforge::graphics::WhiteRubber;
using pixelforge::management::AnimationManager;
using pixelforge::management::CurveManager;
using pixelforge::management::LightManager;
using pixelforge::management::MaterialManager;
using pixelforge::management::MeshManager;
using pixelforge::management::SceneNodeManager;
using pixelforge::management::TextureManager;
using pixelforge::scene::nodes::RenderUnit;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec4Approx(const glm::vec4& actual, const glm::vec4& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
        CHECK(actual.w == doctest::Approx(expected.w).epsilon(EPS));
    }

    struct IntegrationManagersGuard {
        IntegrationManagersGuard() {
            SceneNodeManager::clear();
            AnimationManager::clear();
            CurveManager::clear();
            LightManager::clear();
            MaterialManager::clear();
            MeshManager::clear();
            TextureManager::clear();
        }

        ~IntegrationManagersGuard() {
            TextureManager::clear();
            MeshManager::clear();
            MaterialManager::clear();
            LightManager::clear();
            CurveManager::clear();
            AnimationManager::clear();
            SceneNodeManager::clear();
        }
    };

} // namespace


TEST_CASE("Material texture pipeline integrates with render unit") {
    IntegrationManagersGuard guard;
    TextureManager::initialize();

    auto material = std::make_shared<Material>("test.jpg");
    auto mesh = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);

    RenderUnit unit("RenderA");
    unit.setMesh(mesh);
    unit.setMaterial(material);

    REQUIRE(unit.getMesh() != nullptr);
    REQUIRE(unit.getMaterial() != nullptr);
    REQUIRE(unit.getMaterial()->getDiffuseMap() != nullptr);

    CHECK(unit.getMaterial()->getDiffuseMap()->getFilePath().find("test.jpg") != std::string::npos);

    static_cast<void>(unit.getMaterial()->getDiffuseMap()->bindTexture());
    CHECK(unit.getMaterial()->getDiffuseMap()->getUnit() != TextureManager::noUnit());

    unit.getMaterial()->unbindTextures();
    CHECK(unit.getMaterial()->getDiffuseMap()->getUnit() == TextureManager::noUnit());
}

TEST_CASE("Equivalent materials reuse canonical diffuse texture through texture manager") {
    IntegrationManagersGuard guard;
    TextureManager::initialize();

    auto materialA = std::make_shared<Material>("test.jpg");
    auto materialB = std::make_shared<Material>("test.jpg");

    REQUIRE(materialA->getDiffuseMap() != nullptr);
    REQUIRE(materialB->getDiffuseMap() != nullptr);

    CHECK(materialA->getDiffuseMap() == materialB->getDiffuseMap());
}

TEST_CASE("Equivalent materials become canonical through material manager") {
    IntegrationManagersGuard guard;
    TextureManager::initialize();

    auto materialA = std::make_shared<Material>("test.jpg");
    auto materialB = std::make_shared<Material>("test.jpg");

    auto managedA = MaterialManager::getOrCreate(materialA);
    auto managedB = MaterialManager::getOrCreate(materialB);

    REQUIRE(managedA != nullptr);
    REQUIRE(managedB != nullptr);
    CHECK(managedA == managedB);
}

TEST_CASE("Render units using equivalent materials share canonical managed material") {
    IntegrationManagersGuard guard;
    TextureManager::initialize();

    auto mesh = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);
    auto materialA = std::make_shared<Material>("test.jpg");
    auto materialB = std::make_shared<Material>("test.jpg");

    RenderUnit unitA("RenderA");
    RenderUnit unitB("RenderB");

    unitA.setMesh(mesh);
    unitB.setMesh(mesh);

    unitA.setMaterial(materialA);
    unitB.setMaterial(materialB);

    REQUIRE(unitA.getMaterial() != nullptr);
    REQUIRE(unitB.getMaterial() != nullptr);

    CHECK(unitA.getMaterial() == unitB.getMaterial());
}

TEST_CASE("Material with explicit diffuse and specular maps unbinds both textures") {
    IntegrationManagersGuard guard;
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

TEST_CASE("Render unit retains mesh and managed material together") {
    IntegrationManagersGuard guard;
    TextureManager::initialize();

    auto mesh = std::make_shared<Cuboid>(2.0f, 3.0f, 4.0f);
    auto material = std::make_shared<Material>("test.jpg");

    RenderUnit unit("RenderA");
    unit.setMesh(mesh);
    unit.setMaterial(material);

    REQUIRE(unit.getMesh() != nullptr);
    REQUIRE(unit.getMaterial() != nullptr);

    CHECK(unit.getMesh()->getVertexCount() == mesh->getVertexCount());
    REQUIRE(unit.getMaterial()->getDiffuseMap() != nullptr);
    CHECK(unit.getMaterial()->getDiffuseMap()->getFilePath().find("test.jpg") != std::string::npos);
}

TEST_CASE("Preset material can be attached to render unit and keeps material properties") {
    IntegrationManagersGuard guard;

    auto mesh = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);
    auto material = std::make_shared<WhiteRubber>();

    RenderUnit unit("RenderA");
    unit.setMesh(mesh);
    unit.setMaterial(material);

    REQUIRE(unit.getMaterial() != nullptr);

    checkVec4Approx(unit.getMaterial()->getAmbient(), glm::vec4(0.0500f, 0.0500f, 0.0500f, 1.0000f));
    checkVec4Approx(unit.getMaterial()->getDiffuse(), glm::vec4(0.5000f, 0.5000f, 0.5000f, 1.0000f));
    checkVec4Approx(unit.getMaterial()->getSpecular(), glm::vec4(0.7000f, 0.7000f, 0.7000f, 1.0000f));
    CHECK(unit.getMaterial()->getShininess() == doctest::Approx(10.0f));
}

TEST_CASE("Material pipeline supports manual color properties alongside texture map") {
    IntegrationManagersGuard guard;
    TextureManager::initialize();

    Material material("test.jpg");
    material.setAmbientColor(glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    material.setDiffuseColor(glm::vec4(0.4f, 0.5f, 0.6f, 1.0f));
    material.setSpecularColor(glm::vec4(0.7f, 0.8f, 0.9f, 1.0f));
    material.setShininess(64.0f);

    REQUIRE(material.getDiffuseMap() != nullptr);
    checkVec4Approx(material.getAmbient(), glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    checkVec4Approx(material.getDiffuse(), glm::vec4(0.4f, 0.5f, 0.6f, 1.0f));
    checkVec4Approx(material.getSpecular(), glm::vec4(0.7f, 0.8f, 0.9f, 1.0f));
    CHECK(material.getShininess() == doctest::Approx(64.0f));
}

TEST_CASE("Render unit texture shortcut creates usable material texture pipeline") {
    IntegrationManagersGuard guard;
    TextureManager::initialize();

    RenderUnit unit("RenderA");
    unit.setMesh(std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f));
    unit.setTexture("test.jpg");

    REQUIRE(unit.getMaterial() != nullptr);
    REQUIRE(unit.getMaterial()->getDiffuseMap() != nullptr);

    static_cast<void>(unit.getMaterial()->getDiffuseMap()->bindTexture());
    CHECK(unit.getMaterial()->getDiffuseMap()->getUnit() != TextureManager::noUnit());

    unit.getMaterial()->unbindTextures();
    CHECK(unit.getMaterial()->getDiffuseMap()->getUnit() == TextureManager::noUnit());
}
