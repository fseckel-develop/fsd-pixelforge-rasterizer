#include <doctest.h>
#include <pixelforge/builders/light.hpp>
#include <pixelforge/builders/light_unit.hpp>
#include <pixelforge/builders/model.hpp>
#include <pixelforge/builders/render_unit.hpp>
#include <pixelforge/builders/scene.hpp>
#include <pixelforge/builders/specification/scene_spec.hpp>
#include <managers/light_manager.hpp>
#include <managers/material_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <managers/scene_node_manager.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <memory>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::DirectionalLight;
using pixelforge::builders::LightUnit;
using pixelforge::builders::Model;
using pixelforge::builders::RenderUnit;
using pixelforge::builders::Scene;
using pixelforge::builders::specification::SceneSpec;
using pixelforge::management::LightManager;
using pixelforge::management::MaterialManager;
using pixelforge::management::MeshManager;
using pixelforge::management::SceneNodeManager;
using pixelforge::scene::builders::compiler::buildScene;


namespace {

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


TEST_CASE("Scene builder builds spec with configured name") {
    BuilderManagersGuard guard;

    const auto spec = Scene("SceneA")
        .name("SceneRenamed")
        .buildSpec();

    CHECK(spec.name == "SceneRenamed");
    CHECK(spec.lightUnits.empty());
    CHECK(spec.models.empty());
}

TEST_CASE("Scene builder adds nested top-level light unit via lambda") {
    BuilderManagersGuard guard;

    const auto spec = Scene("SceneA")
        .lightUnit("LightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight().direction(glm::vec3(0.0f, -1.0f, 0.0f)));
        })
        .buildSpec();

    REQUIRE(spec.lightUnits.size() == 1);
    CHECK(spec.lightUnits[0].name == "LightA");
    REQUIRE(spec.lightUnits[0].light.has_value());
}

TEST_CASE("Scene builder adds prebuilt top-level light unit spec") {
    BuilderManagersGuard guard;

    const auto lightSpec = LightUnit("LightA")
        .light(AmbientLight())
        .buildSpec();

    const auto spec = Scene("SceneA")
        .add(lightSpec)
        .buildSpec();

    REQUIRE(spec.lightUnits.size() == 1);
    CHECK(spec.lightUnits[0].name == "LightA");
}

TEST_CASE("Scene builder adds nested model via lambda") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);

    const auto spec = Scene("SceneA")
        .model("ModelA", [&](auto& model) {
            model.renderUnit("RenderA", [&](auto& renderUnit) {
                renderUnit.mesh(mesh);
            });
        })
        .buildSpec();

    REQUIRE(spec.models.size() == 1);
    CHECK(spec.models[0].name == "ModelA");
    REQUIRE(spec.models[0].renderUnits.size() == 1);
    CHECK(spec.models[0].renderUnits[0].name == "RenderA");
}

TEST_CASE("Scene builder adds prebuilt model spec") {
    BuilderManagersGuard guard;

    const auto modelSpec = Model("ModelA")
        .lightUnit("LightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight());
        })
        .buildSpec();

    const auto spec = Scene("SceneA")
        .add(modelSpec)
        .buildSpec();

    REQUIRE(spec.models.size() == 1);
    CHECK(spec.models[0].name == "ModelA");
}

TEST_CASE("Scene compiler builds runtime scene with name") {
    BuilderManagersGuard guard;

    const auto spec = Scene("SceneA").buildSpec();

    const auto scene = buildScene(spec);
    REQUIRE(scene != nullptr);

    CHECK(scene->getName() == "SceneA");
}

TEST_CASE("Scene compiler clears SceneNodeManager before building") {
    BuilderManagersGuard guard;

    const auto stale = std::make_shared<pixelforge::scene::nodes::Scene>("OldScene");
    SceneNodeManager::registerNode(stale);
    REQUIRE(SceneNodeManager::getNodeByName("OldScene") == stale);

    const auto scene = buildScene(Scene("NewScene").buildSpec());
    REQUIRE(scene != nullptr);

    CHECK(SceneNodeManager::getNodeByName("OldScene") == nullptr);
    CHECK(SceneNodeManager::getNodeByName("NewScene") == scene);
}

TEST_CASE("Scene compiler builds and adds top-level light units") {
    BuilderManagersGuard guard;

    const auto spec = Scene("SceneA")
        .lightUnit("LightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight());
        })
        .lightUnit("LightB", [](auto& lightUnit) {
            lightUnit.light(AmbientLight());
        })
        .buildSpec();

    const auto scene = buildScene(spec);
    REQUIRE(scene != nullptr);

    const auto& lights = scene->getGlobalLightUnits();
    REQUIRE(lights.size() == 2);
    CHECK(lights[0]->getName() == "LightA");
    CHECK(lights[1]->getName() == "LightB");
    CHECK(lights[0]->getParent() == scene);
    CHECK(lights[1]->getParent() == scene);
}

TEST_CASE("Scene compiler builds and adds top-level models") {
    BuilderManagersGuard guard;

    const auto meshA = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);
    const auto meshB = std::make_shared<pixelforge::geometry::Cuboid>(2.0f, 2.0f, 2.0f);

    const auto spec = Scene("SceneA")
        .model("ModelA", [&](auto& model) {
            model.renderUnit("RenderA", [&](auto& renderUnit) {
                renderUnit.mesh(meshA);
            });
        })
        .model("ModelB", [&](auto& model) {
            model.renderUnit("RenderB", [&](auto& renderUnit) {
                renderUnit.mesh(meshB);
            });
        })
        .buildSpec();

    const auto scene = buildScene(spec);
    REQUIRE(scene != nullptr);

    const auto& models = scene->getModels();
    REQUIRE(models.size() == 2);
    CHECK(models[0]->getName() == "ModelA");
    CHECK(models[1]->getName() == "ModelB");
    CHECK(models[0]->getParent() == scene);
    CHECK(models[1]->getParent() == scene);
}

TEST_CASE("Scene compiler builds full scene hierarchy with models render units and lights") {
    BuilderManagersGuard guard;

    const auto mesh = std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f);

    const auto spec = Scene("SceneA")
        .lightUnit("GlobalLight", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight());
        })
        .model("ModelA", [&](auto& model) {
            model.lightUnit("ModelLight", [](auto& lightUnit) {
                lightUnit.light(AmbientLight());
            });
            model.renderUnit("RenderA", [&](auto& renderUnit) {
                renderUnit.mesh(mesh);
            });
        })
        .buildSpec();

    const auto scene = buildScene(spec);
    REQUIRE(scene != nullptr);

    REQUIRE(scene->getGlobalLightUnits().size() == 1);
    CHECK(scene->getGlobalLightUnits()[0]->getName() == "GlobalLight");

    REQUIRE(scene->getModels().size() == 1);
    const auto model = scene->getModels()[0];
    REQUIRE(model != nullptr);
    CHECK(model->getName() == "ModelA");

    REQUIRE(model->getLightUnits().size() == 1);
    CHECK(model->getLightUnits()[0]->getName() == "ModelLight");

    REQUIRE(model->getRenderUnits().size() == 1);
    CHECK(model->getRenderUnits()[0]->getName() == "RenderA");

    const auto allLights = scene->getAllLightUnits();
    REQUIRE(allLights.size() == 2);
    CHECK(allLights[0]->getName() == "GlobalLight");
    CHECK(allLights[1]->getName() == "ModelLight");
}

TEST_CASE("Scene builder build compiles directly to runtime scene") {
    BuilderManagersGuard guard;

    const auto scene = Scene("SceneA")
        .lightUnit("LightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight());
        })
        .build();

    REQUIRE(scene != nullptr);
    CHECK(scene->getName() == "SceneA");
    REQUIRE(scene->getGlobalLightUnits().size() == 1);
}

TEST_CASE("Scene builder implicit conversion builds runtime scene") {
    BuilderManagersGuard guard;

    std::shared_ptr<pixelforge::scene::nodes::Scene> scene =
        Scene("SceneA").model("ModelA", [](auto& model) {
            model.lightUnit("LightA", [](auto& lightUnit) {
                lightUnit.light(AmbientLight());
            });
        });

    REQUIRE(scene != nullptr);
    CHECK(scene->getName() == "SceneA");
    REQUIRE(scene->getModels().size() == 1);
}
