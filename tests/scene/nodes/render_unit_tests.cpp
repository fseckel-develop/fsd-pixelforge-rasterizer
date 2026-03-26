#include <doctest.h>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/graphics/texturing/material.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <managers/mesh_manager.hpp>
#include <managers/material_manager.hpp>
#include <GLM/glm.hpp>
#include <memory>

using pixelforge::geometry::Cuboid;
using pixelforge::graphics::Material;
using pixelforge::scene::nodes::RenderUnit;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkMat4Approx(const glm::mat4& actual, const glm::mat4& expected) {
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                CHECK(actual[c][r] == doctest::Approx(expected[c][r]).epsilon(EPS));
            }
        }
    }

    struct RenderUnitManagersGuard {
        RenderUnitManagersGuard() {
            pixelforge::management::MeshManager::clear();
            pixelforge::management::MaterialManager::clear();
        }

        ~RenderUnitManagersGuard() {
            pixelforge::management::MaterialManager::clear();
            pixelforge::management::MeshManager::clear();
        }
    };

} // namespace


TEST_CASE("RenderUnit default constructor starts without mesh and material") {
    RenderUnitManagersGuard guard;

    const RenderUnit unit("RenderUnit");

    CHECK(unit.getMesh() == nullptr);
    CHECK(unit.getMaterial() == nullptr);
}

TEST_CASE("RenderUnit mesh constructor stores mesh") {
    RenderUnitManagersGuard guard;

    const auto mesh = std::make_shared<Cuboid>(2.0f, 3.0f, 4.0f);

    const RenderUnit unit("RenderUnit", mesh);

    REQUIRE(unit.getMesh() != nullptr);
    CHECK(unit.getMesh() == mesh);
    CHECK(unit.getMaterial() == nullptr);
}

TEST_CASE("RenderUnit setMesh updates mesh") {
    RenderUnitManagersGuard guard;

    RenderUnit unit("RenderUnit");

    const auto meshA = std::make_shared<Cuboid>(1.0f, 2.0f, 3.0f);
    const auto meshB = std::make_shared<Cuboid>(4.0f, 5.0f, 6.0f);

    unit.setMesh(meshA);
    REQUIRE(unit.getMesh() != nullptr);
    CHECK(unit.getMesh()->getVertexCount() == meshA->getVertexCount());
    CHECK(unit.getMesh()->getIndices() == meshA->getIndices());

    unit.setMesh(meshB);
    REQUIRE(unit.getMesh() != nullptr);
    CHECK(unit.getMesh()->getVertexCount() == meshB->getVertexCount());
    CHECK(unit.getMesh()->getIndices() == meshB->getIndices());
}

TEST_CASE("RenderUnit setMaterial updates material") {
    RenderUnitManagersGuard guard;

    RenderUnit unit("RenderUnit");

    const auto materialA = std::make_shared<Material>();
    const auto materialB = std::make_shared<Material>();

    materialA->setShininess(12.0f);
    materialB->setShininess(42.0f);

    unit.setMaterial(materialA);
    REQUIRE(unit.getMaterial() != nullptr);
    CHECK(unit.getMaterial() == materialA);
    CHECK(unit.getMaterial()->getShininess() == doctest::Approx(12.0f));

    unit.setMaterial(materialB);
    REQUIRE(unit.getMaterial() != nullptr);
    CHECK(unit.getMaterial() == materialB);
    CHECK(unit.getMaterial()->getShininess() == doctest::Approx(42.0f));
}

TEST_CASE("RenderUnit setMaterial stores provided material") {
    RenderUnitManagersGuard guard;

    RenderUnit unit("RenderUnit");

    const auto material = std::make_shared<Material>();
    material->setShininess(32.0f);

    unit.setMaterial(material);

    REQUIRE(unit.getMaterial() != nullptr);
    CHECK(unit.getMaterial() == material);
    CHECK(unit.getMaterial()->getShininess() == doctest::Approx(32.0f));
}

TEST_CASE("RenderUnit still behaves like TransformNode for global transform") {
    RenderUnitManagersGuard guard;

    auto parent = std::make_shared<RenderUnit>("Parent");
    auto child = std::make_shared<RenderUnit>("Child");

    Transform parentTransform;
    parentTransform.setTranslation(1.0f, 2.0f, 3.0f);
    parent->setTransform(parentTransform);

    Transform childTransform;
    childTransform.setTranslation(4.0f, 5.0f, 6.0f);
    child->setTransform(childTransform);

    parent->addChild(child);

    const Transform& global = child->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(5.0f, 7.0f, 9.0f));
}

TEST_CASE("RenderUnit model matrix matches transform node model matrix") {
    RenderUnitManagersGuard guard;

    const auto unit = std::make_shared<RenderUnit>("RenderUnit");

    Transform local;
    local.setTranslation(1.0f, 2.0f, 3.0f);
    local.setScale(2.0f, 3.0f, 4.0f);
    unit->setTransform(local);

    checkMat4Approx(unit->getModelMatrix(), local.toMatrix());
}
