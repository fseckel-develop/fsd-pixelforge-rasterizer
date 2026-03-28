#include <doctest.h>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/scene/lighting/spot_light.hpp>
#include <pixelforge/scene/transform/scale.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <pixelforge/scene/transform/translate.hpp>
#include <pixelforge/geometry/meshes/sphere.hpp>
#include <managers/light_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <GLM/glm.hpp>
#include <memory>

using pixelforge::scene::lighting::DirectionalLight;
using pixelforge::scene::lighting::Light;
using pixelforge::scene::lighting::PositionalLight;
using pixelforge::scene::lighting::SpotLight;
using pixelforge::scene::nodes::LightUnit;
using pixelforge::scene::nodes::TransformNode;
using pixelforge::scene::transform::Scale;
using pixelforge::scene::transform::Transform;
using pixelforge::scene::transform::Translate;
using pixelforge::geometry::Sphere;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

    void checkMat4Approx(const glm::mat4& actual, const glm::mat4& expected) {
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                CHECK(actual[c][r] == doctest::Approx(expected[c][r]).epsilon(EPS));
            }
        }
    }

    struct LightUnitManagersGuard {
        LightUnitManagersGuard() {
            pixelforge::management::LightManager::clear();
            pixelforge::management::MeshManager::clear();
        }

        ~LightUnitManagersGuard() {
            pixelforge::management::MeshManager::clear();
            pixelforge::management::LightManager::clear();
        }
    };

} // namespace


TEST_CASE("LightUnit default constructor starts with default light and without mesh") {
    LightUnitManagersGuard guard;

    LightUnit unit("LightUnit");

    CHECK(unit.getLight() != nullptr);
    CHECK(unit.getMesh() == nullptr);
}

TEST_CASE("LightUnit constructor with light stores light and leaves mesh unset") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<DirectionalLight>();

    LightUnit unit("LightUnit", light);

    REQUIRE(unit.getLight() != nullptr);
    CHECK(unit.getLight()->getType() == Light::DIRECTIONAL);
    CHECK(unit.getMesh() == nullptr);
}

TEST_CASE("LightUnit setLight replaces light without assigning mesh") {
    LightUnitManagersGuard guard;

    LightUnit unit("LightUnit");

    const auto positional = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));
    unit.setLight(positional);

    REQUIRE(unit.getLight() != nullptr);
    CHECK(unit.getLight()->getType() == Light::POSITIONAL);
    CHECK(unit.getMesh() == nullptr);

    const auto spot = std::make_shared<SpotLight>(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    unit.setLight(spot);

    REQUIRE(unit.getLight() != nullptr);
    CHECK(unit.getLight()->getType() == Light::SPOT);
    CHECK(unit.getMesh() == nullptr);
}

TEST_CASE("LightUnit setMesh stores explicitly assigned mesh") {
    LightUnitManagersGuard guard;

    LightUnit unit("LightUnit");
    CHECK(unit.getMesh() == nullptr);

    unit.setMesh(std::make_shared<Sphere>());

    CHECK(unit.getMesh() != nullptr);
}

TEST_CASE("LightUnit setMesh nullptr clears mesh") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<DirectionalLight>();
    LightUnit unit("LightUnit", light);

    unit.setMesh(std::make_shared<Sphere>());
    REQUIRE(unit.getMesh() != nullptr);

    unit.setMesh(nullptr);

    CHECK(unit.getMesh() == nullptr);
}

TEST_CASE("LightUnit getCurrentPosition applies node global transform to light position") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));
    auto unit = std::make_shared<LightUnit>("LightUnit", light);

    Transform local;
    local.setTranslation(10.0f, 20.0f, 30.0f);
    unit->setTransform(local);

    checkVec3Approx(unit->getCurrentPosition(), glm::vec3(11.0f, 22.0f, 33.0f));
}

TEST_CASE("LightUnit getCurrentPosition includes ancestor transform") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));
    auto parent = std::make_shared<TransformNode>("Parent");
    auto unit = std::make_shared<LightUnit>("LightUnit", light);

    Transform parentTransform;
    parentTransform.setTranslation(5.0f, 0.0f, 0.0f);
    parent->setTransform(parentTransform);
    parent->addChild(unit);

    Transform local;
    local.setTranslation(10.0f, 20.0f, 30.0f);
    unit->setTransform(local);

    checkVec3Approx(unit->getCurrentPosition(), glm::vec3(16.0f, 22.0f, 33.0f));
}

TEST_CASE("LightUnit getCurrentDirection applies global rotation to light direction") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<DirectionalLight>(glm::vec3(1.0f, 0.0f, 0.0f));
    auto unit = std::make_shared<LightUnit>("LightUnit", light);

    Transform local;
    local.setRotation(0.0f, 0.0f, 90.0f);
    unit->setTransform(local);

    checkVec3Approx(unit->getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f), 1e-4f);
}

TEST_CASE("LightUnit getCurrentDirection includes ancestor rotation") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<DirectionalLight>(glm::vec3(1.0f, 0.0f, 0.0f));
    auto parent = std::make_shared<TransformNode>("Parent");
    auto unit = std::make_shared<LightUnit>("LightUnit", light);

    Transform parentTransform;
    parentTransform.setRotation(0.0f, 0.0f, 90.0f);
    parent->setTransform(parentTransform);
    parent->addChild(unit);

    checkVec3Approx(unit->getCurrentDirection(), glm::vec3(0.0f, 1.0f, 0.0f), 1e-4f);
}

TEST_CASE("LightUnit getModelMatrix combines global transform node scale and light position") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<PositionalLight>(glm::vec3(1.0f, 2.0f, 3.0f));
    auto unit = std::make_shared<LightUnit>("LightUnit", light);

    Transform local;
    local.setTranslation(10.0f, 20.0f, 30.0f);
    unit->setTransform(local);
    unit->setNodeScale(glm::vec3(2.0f, 3.0f, 4.0f));

    const glm::mat4 actual = unit->getModelMatrix();

    const Transform expectedTransform =
        local * Scale(glm::vec3(2.0f, 3.0f, 4.0f)) * Translate(glm::vec3(1.0f, 2.0f, 3.0f));

    checkMat4Approx(actual, expectedTransform.toMatrix());
}

TEST_CASE("LightUnit getModelMatrix includes ancestor transform") {
    LightUnitManagersGuard guard;

    const auto light = std::make_shared<PositionalLight>(glm::vec3(1.0f, 0.0f, 0.0f));
    auto parent = std::make_shared<TransformNode>("Parent");
    auto unit = std::make_shared<LightUnit>("LightUnit", light);

    Transform parentTransform;
    parentTransform.setTranslation(5.0f, 0.0f, 0.0f);
    parent->setTransform(parentTransform);
    parent->addChild(unit);

    Transform local;
    local.setTranslation(10.0f, 0.0f, 0.0f);
    unit->setTransform(local);
    unit->setNodeScale(2.0f);

    const glm::mat4 actual = unit->getModelMatrix();

    const Transform expectedTransform =
        parentTransform * local * Scale(2.0f) * Translate(glm::vec3(1.0f, 0.0f, 0.0f));

    checkMat4Approx(actual, expectedTransform.toMatrix());
}
