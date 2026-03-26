#include <doctest.h>
#include <pixelforge/builders/render_unit.hpp>
#include <pixelforge/builders/specification/render_unit_spec.hpp>
#include <managers/scene_node_manager.hpp>
#include <managers/material_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/graphics/texturing/material.hpp>
#include <pixelforge/scene/nodes/group.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <string>

using pixelforge::builders::Orbiting;
using pixelforge::builders::RenderUnit;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scaling;
using pixelforge::builders::Translation;
using pixelforge::builders::specification::RenderUnitSpec;
using pixelforge::management::SceneNodeManager;
using pixelforge::management::MeshManager;
using pixelforge::management::MaterialManager;
using pixelforge::scene::builders::compiler::buildRenderUnit;
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
            MeshManager::clear();
            MaterialManager::clear();
        }

        ~BuilderManagersGuard() {
            MaterialManager::clear();
            MeshManager::clear();
            SceneNodeManager::clear();
        }
    };

} // namespace


TEST_CASE("RenderUnit builder builds spec with configured scalar properties") {
    BuilderManagersGuard guard;

    Transform transform;
    transform.setTranslation(1.0f, 2.0f, 3.0f);

    const auto spec = RenderUnit("RenderA", "ParentA")
        .name("RenderRenamed")
        .parent("ParentRenamed")
        .transform(transform)
        .nodeScale(2.0f)
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP).totalAngle(90.0f))
        .buildSpec();

    CHECK(spec.name == "RenderRenamed");
    CHECK(spec.parentName == "ParentRenamed");

    REQUIRE(spec.transform.has_value());
    checkVec3Approx(spec.transform->getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(spec.uniformNodeScale.has_value());
    CHECK(*spec.uniformNodeScale == doctest::Approx(2.0f));
    CHECK_FALSE(spec.nodeScale.has_value());

    REQUIRE(spec.animations.size() == 1);
}

TEST_CASE("RenderUnit builder vector nodeScale clears scalar nodeScale") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA")
        .nodeScale(2.0f)
        .nodeScale(glm::vec3(3.0f, 4.0f, 5.0f))
        .buildSpec();

    CHECK_FALSE(spec.uniformNodeScale.has_value());
    REQUIRE(spec.nodeScale.has_value());
    checkVec3Approx(*spec.nodeScale, glm::vec3(3.0f, 4.0f, 5.0f));
}

TEST_CASE("RenderUnit builder scalar nodeScale clears vector nodeScale") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA")
        .nodeScale(glm::vec3(3.0f, 4.0f, 5.0f))
        .nodeScale(2.0f)
        .buildSpec();

    REQUIRE(spec.uniformNodeScale.has_value());
    CHECK(*spec.uniformNodeScale == doctest::Approx(2.0f));
    CHECK_FALSE(spec.nodeScale.has_value());
}

TEST_CASE("RenderUnit builder stores mesh shared pointer and clears mesh file name") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 2.0f, 3.0f);

    const auto spec = RenderUnit("RenderA")
        .mesh("old.obj")
        .mesh(mesh)
        .buildSpec();

    CHECK(spec.mesh == mesh);
    CHECK(spec.meshFileName.empty());
}

TEST_CASE("RenderUnit builder stores copied mesh value and clears mesh file name") {
    BuilderManagersGuard guard;

    pixelforge::geometry::Cuboid meshValue(1.0f, 2.0f, 3.0f);

    const auto spec = RenderUnit("RenderA")
        .mesh("old.obj")
        .mesh(meshValue)
        .buildSpec();

    REQUIRE(spec.mesh != nullptr);
    CHECK(spec.meshFileName.empty());
    CHECK(spec.mesh->getVertexCount() == meshValue.getVertexCount());
    CHECK(spec.mesh->getIndices() == meshValue.getIndices());
}

TEST_CASE("RenderUnit builder stores mesh file name and clears mesh pointer") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 2.0f, 3.0f);

    const auto spec = RenderUnit("RenderA")
        .mesh(mesh)
        .mesh("model.obj")
        .buildSpec();

    CHECK(spec.mesh == nullptr);
    CHECK(spec.meshFileName == "model.obj");
}

TEST_CASE("RenderUnit builder stores material shared pointer and clears texture file name") {
    BuilderManagersGuard guard;

    const auto material = std::make_shared<pixelforge::graphics::Material>();
    material->setShininess(32.0f);

    const auto spec = RenderUnit("RenderA")
        .texture("old.png")
        .material(material)
        .buildSpec();

    CHECK(spec.material == material);
    CHECK(spec.textureFileName.empty());
}

TEST_CASE("RenderUnit builder stores copied material value and clears texture file name") {
    BuilderManagersGuard guard;

    pixelforge::graphics::Material materialValue;
    materialValue.setShininess(64.0f);

    const auto spec = RenderUnit("RenderA")
        .texture("old.png")
        .material(materialValue)
        .buildSpec();

    REQUIRE(spec.material != nullptr);
    CHECK(spec.textureFileName.empty());
    CHECK(spec.material->getShininess() == doctest::Approx(64.0f));
}

TEST_CASE("RenderUnit builder stores texture file name without clearing material pointer") {
    BuilderManagersGuard guard;

    const auto material = std::make_shared<pixelforge::graphics::Material>();
    material->setShininess(16.0f);

    const auto spec = RenderUnit("RenderA")
        .material(material)
        .texture("albedo.png")
        .buildSpec();

    CHECK(spec.material == material);
    CHECK(spec.textureFileName == "albedo.png");
}

TEST_CASE("RenderUnit builder stores multiple animations") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA")
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP))
        .animation(Scaling(pixelforge::scene::animation::Animation::BOUNCE).targetScale(2.0f))
        .animation(Translation(pixelforge::scene::animation::Animation::ONCE).totalDistance(5.0f))
        .animation(Orbiting(pixelforge::scene::animation::Animation::LOOP).radius(3.0f))
        .buildSpec();

    REQUIRE(spec.animations.size() == 4);
}

TEST_CASE("RenderUnit compiler builds runtime node with name") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA").buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);

    CHECK(renderUnit->getName() == "RenderA");
}

TEST_CASE("RenderUnit compiler registers built node in SceneNodeManager") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA").buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);

    CHECK(SceneNodeManager::getNodeByName("RenderA") == renderUnit);
}

TEST_CASE("RenderUnit compiler assigns parent when parent name is provided") {
    BuilderManagersGuard guard;

    const auto parent = std::make_shared<Group>("ParentGroup");
    SceneNodeManager::registerNode(parent);

    const auto spec = RenderUnit("RenderA", "ParentGroup").buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);

    CHECK(renderUnit->getParent() == parent);

    const auto children = parent->getChildren();
    REQUIRE(children.size() == 1);
    CHECK(children[0] == renderUnit);
}

TEST_CASE("RenderUnit compiler applies local transform") {
    BuilderManagersGuard guard;

    Transform transform;
    transform.setTranslation(1.0f, 2.0f, 3.0f);
    transform.setScale(2.0f, 3.0f, 4.0f);

    const auto spec = RenderUnit("RenderA")
        .transform(transform)
        .buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);

    const auto global = renderUnit->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(global.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("RenderUnit compiler applies uniform node scale") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA")
        .nodeScale(2.0f)
        .buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);

    const auto modelMatrix = renderUnit->getModelMatrix();
    CHECK(modelMatrix[0][0] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[1][1] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[2][2] == doctest::Approx(2.0f).epsilon(EPS));
}

TEST_CASE("RenderUnit compiler applies non-uniform node scale") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA")
        .nodeScale(glm::vec3(2.0f, 3.0f, 4.0f))
        .buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);

    const auto modelMatrix = renderUnit->getModelMatrix();
    CHECK(modelMatrix[0][0] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[1][1] == doctest::Approx(3.0f).epsilon(EPS));
    CHECK(modelMatrix[2][2] == doctest::Approx(4.0f).epsilon(EPS));
}

TEST_CASE("RenderUnit compiler applies explicit mesh override") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 2.0f, 3.0f);

    const auto spec = RenderUnit("RenderA")
        .mesh(mesh)
        .buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);
    REQUIRE(renderUnit->getMesh() != nullptr);

    CHECK(renderUnit->getMesh()->getVertexCount() == mesh->getVertexCount());
    CHECK(renderUnit->getMesh()->getIndices() == mesh->getIndices());
}

TEST_CASE("RenderUnit compiler applies explicit material override") {
    BuilderManagersGuard guard;

    const auto material = std::make_shared<pixelforge::graphics::Material>();
    material->setShininess(32.0f);

    const auto spec = RenderUnit("RenderA")
        .material(material)
        .buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);
    REQUIRE(renderUnit->getMaterial() != nullptr);

    CHECK(renderUnit->getMaterial()->getShininess() == doctest::Approx(32.0f));
}

TEST_CASE("RenderUnit compiler adds animations through transform node compiler utilities") {
    BuilderManagersGuard guard;

    const auto spec = RenderUnit("RenderA")
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP).totalAngle(90.0f))
        .animation(Translation(pixelforge::scene::animation::Animation::ONCE).totalDistance(5.0f))
        .buildSpec();

    const auto renderUnit = buildRenderUnit(spec);
    REQUIRE(renderUnit != nullptr);

    CHECK(renderUnit->isGlobalTransformDirty());
    static_cast<void>(renderUnit->getGlobalTransform());
    CHECK_FALSE(renderUnit->isGlobalTransformDirty());
}

TEST_CASE("RenderUnit builder build compiles directly to runtime node") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);

    const auto renderUnit = RenderUnit("RenderA")
        .mesh(mesh)
        .build();

    REQUIRE(renderUnit != nullptr);
    CHECK(renderUnit->getName() == "RenderA");
    REQUIRE(renderUnit->getMesh() != nullptr);
}

TEST_CASE("RenderUnit builder implicit conversion builds runtime node") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);

    std::shared_ptr<pixelforge::scene::nodes::RenderUnit> renderUnit =
        RenderUnit("RenderA").mesh(mesh);

    REQUIRE(renderUnit != nullptr);
    CHECK(renderUnit->getName() == "RenderA");
    REQUIRE(renderUnit->getMesh() != nullptr);
}
