#include <doctest.h>
#include <pixelforge/builders/light.hpp>
#include <pixelforge/builders/light_unit.hpp>
#include <pixelforge/builders/model.hpp>
#include <pixelforge/builders/render_unit.hpp>
#include <pixelforge/builders/specification/model_spec.hpp>
#include <managers/light_manager.hpp>
#include <managers/material_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <managers/scene_node_manager.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/scene/nodes/group.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::DirectionalLight;
using pixelforge::builders::LightUnit;
using pixelforge::builders::Model;
using pixelforge::builders::Orbiting;
using pixelforge::builders::RenderUnit;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scaling;
using pixelforge::builders::Translation;
using pixelforge::builders::specification::ModelSpec;
using pixelforge::management::LightManager;
using pixelforge::management::MaterialManager;
using pixelforge::management::MeshManager;
using pixelforge::management::SceneNodeManager;
using pixelforge::scene::builders::compiler::buildModel;
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
            MaterialManager::clear();
        }

        ~BuilderManagersGuard() {
            MaterialManager::clear();
            MeshManager::clear();
            LightManager::clear();
            SceneNodeManager::clear();
        }
    };

} // namespace


TEST_CASE("Model builder builds spec with configured scalar properties") {
    BuilderManagersGuard guard;

    Transform transform;
    transform.setTranslation(1.0f, 2.0f, 3.0f);

    const auto spec = Model("ModelA", "ParentA")
        .name("ModelRenamed")
        .parent("ParentRenamed")
        .transform(transform)
        .nodeScale(2.0f)
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP).totalAngle(90.0f))
        .buildSpec();

    CHECK(spec.name == "ModelRenamed");
    CHECK(spec.parentName == "ParentRenamed");

    REQUIRE(spec.transform.has_value());
    checkVec3Approx(spec.transform->getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));

    REQUIRE(spec.uniformNodeScale.has_value());
    CHECK(*spec.uniformNodeScale == doctest::Approx(2.0f));
    CHECK_FALSE(spec.nodeScale.has_value());

    REQUIRE(spec.animations.size() == 1);
    CHECK(spec.lightUnits.empty());
    CHECK(spec.renderUnits.empty());
}

TEST_CASE("Model builder vector nodeScale clears scalar nodeScale") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .nodeScale(2.0f)
        .nodeScale(glm::vec3(3.0f, 4.0f, 5.0f))
        .buildSpec();

    CHECK_FALSE(spec.uniformNodeScale.has_value());
    REQUIRE(spec.nodeScale.has_value());
    checkVec3Approx(*spec.nodeScale, glm::vec3(3.0f, 4.0f, 5.0f));
}

TEST_CASE("Model builder scalar nodeScale clears vector nodeScale") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .nodeScale(glm::vec3(3.0f, 4.0f, 5.0f))
        .nodeScale(2.0f)
        .buildSpec();

    REQUIRE(spec.uniformNodeScale.has_value());
    CHECK(*spec.uniformNodeScale == doctest::Approx(2.0f));
    CHECK_FALSE(spec.nodeScale.has_value());
}

TEST_CASE("Model builder stores multiple animations") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP))
        .animation(Scaling(pixelforge::scene::animation::Animation::BOUNCE).targetScale(2.0f))
        .animation(Translation(pixelforge::scene::animation::Animation::ONCE).totalDistance(5.0f))
        .animation(Orbiting(pixelforge::scene::animation::Animation::LOOP).radius(3.0f))
        .buildSpec();

    REQUIRE(spec.animations.size() == 4);
}

TEST_CASE("Model builder adds nested light unit via lambda") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .lightUnit("LightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight().direction(glm::vec3(0.0f, -1.0f, 0.0f)));
        })
        .buildSpec();

    REQUIRE(spec.lightUnits.size() == 1);
    CHECK(spec.lightUnits[0].name == "LightA");
    REQUIRE(spec.lightUnits[0].light.has_value());
}

TEST_CASE("Model builder adds prebuilt light unit spec") {
    BuilderManagersGuard guard;

    const auto lightSpec = LightUnit("LightA")
        .light(AmbientLight())
        .buildSpec();

    const auto spec = Model("ModelA")
        .add(lightSpec)
        .buildSpec();

    REQUIRE(spec.lightUnits.size() == 1);
    CHECK(spec.lightUnits[0].name == "LightA");
}

TEST_CASE("Model builder adds nested render unit via lambda") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);

    const auto spec = Model("ModelA")
        .renderUnit("RenderA", [&](auto& renderUnit) {
            renderUnit.mesh(mesh);
        })
        .buildSpec();

    REQUIRE(spec.renderUnits.size() == 1);
    CHECK(spec.renderUnits[0].name == "RenderA");
    CHECK(spec.renderUnits[0].mesh == mesh);
}

TEST_CASE("Model builder adds prebuilt render unit spec") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);

    const auto renderSpec = RenderUnit("RenderA")
        .mesh(mesh)
        .buildSpec();

    const auto spec = Model("ModelA")
        .add(renderSpec)
        .buildSpec();

    REQUIRE(spec.renderUnits.size() == 1);
    CHECK(spec.renderUnits[0].name == "RenderA");
    CHECK(spec.renderUnits[0].mesh == mesh);
}

TEST_CASE("Model compiler builds runtime node with name") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA").buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    CHECK(model->getName() == "ModelA");
}

TEST_CASE("Model compiler registers built node in SceneNodeManager") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA").buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    CHECK(SceneNodeManager::getNodeByName("ModelA") == model);
}

TEST_CASE("Model compiler assigns parent when parent name is provided") {
    BuilderManagersGuard guard;

    const auto parent = std::make_shared<Group>("ParentGroup");
    SceneNodeManager::registerNode(parent);

    const auto spec = Model("ModelA", "ParentGroup").buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    CHECK(model->getParent() == parent);

    const auto children = parent->getChildren();
    REQUIRE(children.size() == 1);
    CHECK(children[0] == model);
}

TEST_CASE("Model compiler applies local transform") {
    BuilderManagersGuard guard;

    Transform transform;
    transform.setTranslation(1.0f, 2.0f, 3.0f);
    transform.setScale(2.0f, 3.0f, 4.0f);

    const auto spec = Model("ModelA")
        .transform(transform)
        .buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    const auto global = model->getGlobalTransform();
    checkVec3Approx(global.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(global.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("Model compiler applies uniform node scale") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .nodeScale(2.0f)
        .buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    const auto modelMatrix = model->getModelMatrix();
    CHECK(modelMatrix[0][0] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[1][1] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[2][2] == doctest::Approx(2.0f).epsilon(EPS));
}

TEST_CASE("Model compiler applies non-uniform node scale") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .nodeScale(glm::vec3(2.0f, 3.0f, 4.0f))
        .buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    const auto modelMatrix = model->getModelMatrix();
    CHECK(modelMatrix[0][0] == doctest::Approx(2.0f).epsilon(EPS));
    CHECK(modelMatrix[1][1] == doctest::Approx(3.0f).epsilon(EPS));
    CHECK(modelMatrix[2][2] == doctest::Approx(4.0f).epsilon(EPS));
}

TEST_CASE("Model compiler builds and adds nested light units") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .lightUnit("LightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight().direction(glm::vec3(0.0f, -1.0f, 0.0f)));
        })
        .lightUnit("LightB", [](auto& lightUnit) {
            lightUnit.light(AmbientLight());
        })
        .buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    const auto& lights = model->getLightUnits();
    REQUIRE(lights.size() == 2);
    CHECK(lights[0]->getName() == "LightA");
    CHECK(lights[1]->getName() == "LightB");
    CHECK(lights[0]->getParent() == model);
    CHECK(lights[1]->getParent() == model);
}

TEST_CASE("Model compiler builds and adds nested render units") {
    BuilderManagersGuard guard;

    const auto meshA = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);
    const auto meshB = std::make_shared<pixelforge::geometry::Cuboid>(2.0f, 2.0f, 2.0f);

    const auto spec = Model("ModelA")
        .renderUnit("RenderA", [&](auto& renderUnit) {
            renderUnit.mesh(meshA);
        })
        .renderUnit("RenderB", [&](auto& renderUnit) {
            renderUnit.mesh(meshB);
        })
        .buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    const auto& renderUnits = model->getRenderUnits();
    REQUIRE(renderUnits.size() == 2);
    CHECK(renderUnits[0]->getName() == "RenderA");
    CHECK(renderUnits[1]->getName() == "RenderB");
    CHECK(renderUnits[0]->getParent() == model);
    CHECK(renderUnits[1]->getParent() == model);
}

TEST_CASE("Model compiler adds animations through transform node compiler utilities") {
    BuilderManagersGuard guard;

    const auto spec = Model("ModelA")
        .animation(Rotation(pixelforge::scene::animation::Animation::LOOP).totalAngle(90.0f))
        .animation(Translation(pixelforge::scene::animation::Animation::ONCE).totalDistance(5.0f))
        .buildSpec();

    const auto model = buildModel(spec);
    REQUIRE(model != nullptr);

    CHECK(model->isGlobalTransformDirty());
    static_cast<void>(model->getGlobalTransform());
    CHECK_FALSE(model->isGlobalTransformDirty());
}

TEST_CASE("Model builder build compiles directly to runtime node") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);

    const auto model = Model("ModelA")
        .renderUnit("RenderA", [&](auto& renderUnit) {
            renderUnit.mesh(mesh);
        })
        .build();

    REQUIRE(model != nullptr);
    CHECK(model->getName() == "ModelA");
    REQUIRE(model->getRenderUnits().size() == 1);
}

TEST_CASE("Model builder implicit conversion builds runtime node") {
    BuilderManagersGuard guard;

    std::shared_ptr<pixelforge::scene::nodes::Model> model =
        Model("ModelA").lightUnit("LightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight());
        });

    REQUIRE(model != nullptr);
    CHECK(model->getName() == "ModelA");
    REQUIRE(model->getLightUnits().size() == 1);
}
