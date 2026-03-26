#include <doctest.h>
#include <pixelforge/builders/light.hpp>
#include <pixelforge/builders/light_unit.hpp>
#include <pixelforge/builders/specification/light_unit_spec.hpp>
#include <managers/scene_node_manager.hpp>
#include <managers/light_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/scene/nodes/group.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <variant>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::DirectionalLight;
using pixelforge::builders::LightUnit;
using pixelforge::builders::Orbiting;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scaling;
using pixelforge::builders::Translation;
using pixelforge::builders::specification::DirectionalLightSpec;
using pixelforge::builders::specification::LightUnitSpec;
using pixelforge::management::SceneNodeManager;
using pixelforge::management::LightManager;
using pixelforge::management::MeshManager;
using pixelforge::scene::builders::compiler::buildLightUnit;
using pixelforge::scene::nodes::Group;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    struct BuilderManagersGuard {
        BuilderManagersGuard() {
            SceneNodeManager::clear();
            LightManager::clear();
            MeshManager::clear();
        }

        ~BuilderManagersGuard() {
            MeshManager::clear();
            LightManager::clear();
            SceneNodeManager::clear();
        }
    };

} // namespace


TEST_CASE("LightUnit builder builds spec with configured scalar properties") {
    BuilderManagersGuard guard;

    Transform transform;
    transform.setTranslation(1.0f, 2.0f, 3.0f);

    const auto spec = LightUnit("LightA", "ParentA")
        .name("LightRenamed")
        .parent("ParentRenamed")
        .transform(transform)
        .nodeScale(2.0f)
        .light(DirectionalLight().direction(glm::vec3(0.0f, -1.0f, 0.0f)))
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP).totalAngle(90.0f))
        .buildSpec();

    CHECK(spec.name == "LightRenamed");
    CHECK(spec.parentName == "ParentRenamed");
    REQUIRE(spec.transform.has_value());
    checkVec3Approx(spec.transform->getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(spec.uniformNodeScale.has_value());
    CHECK(*spec.uniformNodeScale == doctest::Approx(2.0f));
    CHECK_FALSE(spec.nodeScale.has_value());

    REQUIRE(spec.light.has_value());
    CHECK(std::holds_alternative<DirectionalLightSpec>(spec.light->data));

    REQUIRE(spec.animations.size() == 1);
}

TEST_CASE("LightUnit builder nodeScale vector clears scalar nodeScale") {
    BuilderManagersGuard guard;

    const auto spec = LightUnit("LightA")
        .nodeScale(2.0f)
        .nodeScale(glm::vec3(3.0f, 4.0f, 5.0f))
        .buildSpec();

    CHECK_FALSE(spec.uniformNodeScale.has_value());
    REQUIRE(spec.nodeScale.has_value());
    checkVec3Approx(*spec.nodeScale, glm::vec3(3.0f, 4.0f, 5.0f));
}

TEST_CASE("LightUnit builder scalar nodeScale clears vector nodeScale") {
    BuilderManagersGuard guard;

    const auto spec = LightUnit("LightA")
        .nodeScale(glm::vec3(3.0f, 4.0f, 5.0f))
        .nodeScale(2.0f)
        .buildSpec();

    REQUIRE(spec.uniformNodeScale.has_value());
    CHECK(*spec.uniformNodeScale == doctest::Approx(2.0f));
    CHECK_FALSE(spec.nodeScale.has_value());
}

TEST_CASE("LightUnit builder stores mesh and multiple animations") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 2.0f, 3.0f);

    const auto spec = LightUnit("LightA")
        .mesh(mesh)
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP))
        .animation(Scaling(pixelforge::scene::animation::Animation::BOUNCE).targetScale(2.0f))
        .animation(Translation(pixelforge::scene::animation::Animation::ONCE).totalDistance(5.0f))
        .animation(Orbiting(pixelforge::scene::animation::Animation::LOOP).radius(3.0f))
        .buildSpec();

    CHECK(spec.mesh == mesh);
    REQUIRE(spec.animations.size() == 4);
}

TEST_CASE("LightUnit compiler builds runtime node with name and compiled light") {
    BuilderManagersGuard guard;

    const auto spec = LightUnit("LightA")
        .light(DirectionalLight()
            .ambient(glm::vec3(0.1f, 0.2f, 0.3f), 0.4f)
            .diffuse(glm::vec3(0.4f, 0.5f, 0.6f), 0.7f)
            .specular(glm::vec3(0.7f, 0.8f, 0.9f), 1.0f)
            .direction(glm::vec3(0.0f, -2.0f, 0.0f)))
        .buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);

    CHECK(lightUnit->getName() == "LightA");
    REQUIRE(lightUnit->getLight() != nullptr);
    CHECK(lightUnit->getLight()->getType() == pixelforge::scene::lighting::Light::DIRECTIONAL);
    checkVec3Approx(lightUnit->getLight()->getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
}

TEST_CASE("LightUnit compiler registers built node in SceneNodeManager") {
    BuilderManagersGuard guard;

    const auto spec = LightUnit("LightA").buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);

    CHECK(SceneNodeManager::getNodeByName("LightA") == lightUnit);
}

TEST_CASE("LightUnit compiler assigns parent when parent name is provided") {
    BuilderManagersGuard guard;

    const auto parent = std::make_shared<Group>("ParentGroup");
    SceneNodeManager::registerNode(parent);

    const auto spec = LightUnit("LightA", "ParentGroup").buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);

    CHECK(lightUnit->getParent() == parent);

    const auto children = parent->getChildren();
    REQUIRE(children.size() == 1);
    CHECK(children[0] == lightUnit);
}

TEST_CASE("LightUnit compiler applies local transform") {
    BuilderManagersGuard guard;

    Transform transform;
    transform.setTranslation(1.0f, 2.0f, 3.0f);
    transform.setScale(2.0f, 3.0f, 4.0f);

    const auto spec = LightUnit("LightA")
        .transform(transform)
        .buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);

    const auto global = lightUnit->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(global.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("LightUnit compiler applies uniform node scale") {
    BuilderManagersGuard guard;

    const auto spec = LightUnit("LightA")
        .light(DirectionalLight())
        .nodeScale(2.0f)
        .buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);

    const auto modelMatrix = lightUnit->getModelMatrix();
    CHECK(modelMatrix[0][0] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[1][1] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[2][2] == doctest::Approx(2.0f).epsilon(EPS));
}

TEST_CASE("LightUnit compiler applies non-uniform node scale") {
    BuilderManagersGuard guard;

    const auto spec = LightUnit("LightA")
        .light(DirectionalLight())
        .nodeScale(glm::vec3(2.0f, 3.0f, 4.0f))
        .buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);

    const auto modelMatrix = lightUnit->getModelMatrix();
    CHECK(modelMatrix[0][0] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[1][1] == doctest::Approx(3.0f).epsilon(EPS));
    CHECK(modelMatrix[2][2] == doctest::Approx(4.0f).epsilon(EPS));
}

TEST_CASE("LightUnit compiler uses explicit mesh override") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 2.0f, 3.0f);

    const auto spec = LightUnit("LightA")
        .light(DirectionalLight())
        .mesh(mesh)
        .buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);
    REQUIRE(lightUnit->getMesh() != nullptr);

    CHECK(lightUnit->getMesh()->getVertexCount() == mesh->getVertexCount());
    CHECK(lightUnit->getMesh()->getIndices() == mesh->getIndices());
}

TEST_CASE("LightUnit compiler adds animations through transform node compiler utilities") {
    BuilderManagersGuard guard;

    const auto spec = LightUnit("LightA")
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP).totalAngle(90.0f))
        .animation(Translation(pixelforge::scene::animation::Animation::ONCE).totalDistance(5.0f))
        .buildSpec();

    const auto lightUnit = buildLightUnit(spec);
    REQUIRE(lightUnit != nullptr);

    // Smoke test: animations were attached without breaking transform evaluation.
    CHECK(lightUnit->isGlobalTransformDirty());
    static_cast<void>(lightUnit->getGlobalTransform());
    CHECK_FALSE(lightUnit->isGlobalTransformDirty());
}

TEST_CASE("LightUnit builder build compiles directly to runtime node") {
    BuilderManagersGuard guard;

    const auto lightUnit = LightUnit("LightA")
        .light(AmbientLight().ambient(glm::vec3(0.2f, 0.3f, 0.4f), 0.5f))
        .build();

    REQUIRE(lightUnit != nullptr);
    CHECK(lightUnit->getName() == "LightA");
    REQUIRE(lightUnit->getLight() != nullptr);
    CHECK(lightUnit->getLight()->getType() == pixelforge::scene::lighting::Light::AMBIENT);
}

TEST_CASE("LightUnit builder implicit conversion builds runtime node") {
    BuilderManagersGuard guard;

    std::shared_ptr<pixelforge::scene::nodes::LightUnit> lightUnit =
        LightUnit("LightA").light(DirectionalLight());

    REQUIRE(lightUnit != nullptr);
    CHECK(lightUnit->getName() == "LightA");
    REQUIRE(lightUnit->getLight() != nullptr);
    CHECK(lightUnit->getLight()->getType() == pixelforge::scene::lighting::Light::DIRECTIONAL);
}
