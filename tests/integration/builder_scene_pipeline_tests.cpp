#include <doctest.h>
#include <pixelforge/builders/light.hpp>
#include <pixelforge/builders/light_unit.hpp>
#include <pixelforge/builders/model.hpp>
#include <pixelforge/builders/render_unit.hpp>
#include <pixelforge/builders/scene.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <managers/animation_manager.hpp>
#include <managers/curve_manager.hpp>
#include <managers/light_manager.hpp>
#include <managers/material_manager.hpp>
#include <managers/mesh_manager.hpp>
#include <managers/scene_node_manager.hpp>
#include <managers/texture_manager.hpp>
#include <GLM/glm.hpp>
#include <memory>

using pixelforge::builders::AmbientLight;
using pixelforge::builders::DirectionalLight;
using pixelforge::builders::Model;
using pixelforge::builders::RenderUnit;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scene;
using pixelforge::builders::Translation;
using pixelforge::geometry::Cuboid;
using pixelforge::management::AnimationManager;
using pixelforge::management::CurveManager;
using pixelforge::management::LightManager;
using pixelforge::management::MaterialManager;
using pixelforge::management::MeshManager;
using pixelforge::management::SceneNodeManager;
using pixelforge::management::TextureManager;
using pixelforge::scene::nodes::LightUnit;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
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


TEST_CASE("Scene builder compiler produces connected runtime scene graph") {
    IntegrationManagersGuard guard;

    const auto mesh = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);

    const auto scene = Scene("SceneA")
        .lightUnit("GlobalLight", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight().direction(glm::vec3(0.0f, -1.0f, 0.0f)));
        })
        .model("ModelA", [&](auto& model) {
            model.lightUnit("ModelLight", [](auto& lightUnit) {
                lightUnit.light(AmbientLight());
            });
            model.renderUnit("RenderA", [&](auto& renderUnit) {
                renderUnit.mesh(mesh);
            });
        })
        .build();

    REQUIRE(scene != nullptr);
    CHECK(scene->getName() == "SceneA");

    REQUIRE(scene->getGlobalLightUnits().size() == 1);
    REQUIRE(scene->getModels().size() == 1);

    const auto globalLight = scene->getGlobalLightUnits().front();
    REQUIRE(globalLight != nullptr);
    CHECK(globalLight->getName() == "GlobalLight");
    CHECK(globalLight->getParent() == scene);

    const auto model = scene->getModels().front();
    REQUIRE(model != nullptr);
    CHECK(model->getName() == "ModelA");
    CHECK(model->getParent() == scene);

    REQUIRE(model->getLightUnits().size() == 1);
    REQUIRE(model->getRenderUnits().size() == 1);

    const auto modelLight = model->getLightUnits().front();
    const auto renderUnit = model->getRenderUnits().front();

    REQUIRE(modelLight != nullptr);
    REQUIRE(renderUnit != nullptr);

    CHECK(modelLight->getName() == "ModelLight");
    CHECK(modelLight->getParent() == model);

    CHECK(renderUnit->getName() == "RenderA");
    CHECK(renderUnit->getParent() == model);

    const auto allLights = scene->getAllLightUnits();
    REQUIRE(allLights.size() == 2);
    CHECK(allLights[0] == globalLight);
    CHECK(allLights[1] == modelLight);
}

TEST_CASE("Scene builder compiler preserves transforms through runtime hierarchy") {
    IntegrationManagersGuard guard;

    pixelforge::scene::transform::Transform modelTransform;
    modelTransform.setTranslation(1.0f, 2.0f, 3.0f);

    pixelforge::scene::transform::Transform renderTransform;
    renderTransform.setTranslation(4.0f, 5.0f, 6.0f);

    const auto mesh = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);

    const auto scene = Scene("SceneA")
        .model("ModelA", [&](auto& model) {
            model.transform(modelTransform);
            model.renderUnit("RenderA", [&](auto& renderUnit) {
                renderUnit.mesh(mesh);
                renderUnit.transform(renderTransform);
            });
        })
        .build();

    REQUIRE(scene != nullptr);
    REQUIRE(scene->getModels().size() == 1);

    const auto model = scene->getModels().front();
    REQUIRE(model != nullptr);
    REQUIRE(model->getRenderUnits().size() == 1);

    const auto renderUnit = model->getRenderUnits().front();
    REQUIRE(renderUnit != nullptr);

    checkVec3Approx(model->getGlobalTransform().getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(renderUnit->getGlobalTransform().getTranslation(), glm::vec3(5.0f, 7.0f, 9.0f));
}

TEST_CASE("Scene builder compiler integrates animations into runtime nodes") {
    IntegrationManagersGuard guard;

    const auto mesh = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);

    const auto scene = Scene("SceneA")
        .model("ModelA", [&](auto& model) {
            model.animation(Translation(pixelforge::scene::animation::Animation::ONCE)
                                .duration(2.0f)
                                .direction(glm::vec3(1.0f, 0.0f, 0.0f))
                                .totalDistance(10.0f));
            model.renderUnit("RenderA", [&](auto& renderUnit) {
                renderUnit.mesh(mesh);
                renderUnit.animation(Rotation(pixelforge::scene::animation::Animation::LOOP)
                                         .duration(1.0f)
                                         .totalAngle(90.0f));
            });
        })
        .build();

    REQUIRE(scene != nullptr);
    REQUIRE(scene->getModels().size() == 1);

    const auto model = scene->getModels().front();
    REQUIRE(model != nullptr);
    REQUIRE(model->getRenderUnits().size() == 1);

    const auto renderUnit = model->getRenderUnits().front();
    REQUIRE(renderUnit != nullptr);

    CHECK(model->isGlobalTransformDirty());
    scene->updateSelfAndChildren(1.0f);

    const auto modelTransform = model->getGlobalTransform();
    CHECK(modelTransform.getTranslation().x > 0.0f);

    const auto renderMatrix = renderUnit->getModelMatrix();
    CHECK_FALSE(renderMatrix == glm::mat4(1.0f));
}

TEST_CASE("Scene builder compiler and managers canonicalize equivalent mesh resources across nodes") {
    IntegrationManagersGuard guard;

    const auto scene = Scene("SceneA")
        .model("ModelA", [](auto& model) {
            model.renderUnit("RenderA", [](auto& renderUnit) {
                renderUnit.mesh(Cuboid(1.0f, 1.0f, 1.0f));
            });
            model.renderUnit("RenderB", [](auto& renderUnit) {
                renderUnit.mesh(Cuboid(1.0f, 1.0f, 1.0f));
            });
        })
        .build();

    REQUIRE(scene != nullptr);
    REQUIRE(scene->getModels().size() == 1);

    const auto model = scene->getModels().front();
    REQUIRE(model != nullptr);
    REQUIRE(model->getRenderUnits().size() == 2);

    const auto renderA = model->getRenderUnits()[0];
    const auto renderB = model->getRenderUnits()[1];

    REQUIRE(renderA != nullptr);
    REQUIRE(renderB != nullptr);
    REQUIRE(renderA->getMesh() != nullptr);
    REQUIRE(renderB->getMesh() != nullptr);

    CHECK(renderA->getMesh() == renderB->getMesh());
}

TEST_CASE("Scene builder compiler registers top level runtime nodes by name") {
    IntegrationManagersGuard guard;

    const auto scene = Scene("SceneA")
        .lightUnit("GlobalLight", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight());
        })
        .model("ModelA", [](auto& model) {
            model.lightUnit("ModelLight", [](auto& lightUnit) {
                lightUnit.light(AmbientLight());
            });
        })
        .build();

    REQUIRE(scene != nullptr);

    CHECK(SceneNodeManager::getNodeByName("SceneA") == scene);

    const auto modelNode = SceneNodeManager::getNodeByName("ModelA");
    const auto globalLightNode = SceneNodeManager::getNodeByName("GlobalLight");
    const auto modelLightNode = SceneNodeManager::getNodeByName("ModelLight");

    REQUIRE(modelNode != nullptr);
    REQUIRE(globalLightNode != nullptr);
    REQUIRE(modelLightNode != nullptr);

    CHECK(modelNode->getName() == "ModelA");
    CHECK(globalLightNode->getName() == "GlobalLight");
    CHECK(modelLightNode->getName() == "ModelLight");
}

TEST_CASE("Scene builder compiler produces stable scene traversal with global and model lights") {
    IntegrationManagersGuard guard;

    const auto mesh = std::make_shared<Cuboid>(1.0f, 2.0f, 3.0f);

    const auto scene = Scene("SceneA")
        .lightUnit("GlobalLightA", [](auto& lightUnit) {
            lightUnit.light(DirectionalLight());
        })
        .lightUnit("GlobalLightB", [](auto& lightUnit) {
            lightUnit.light(AmbientLight());
        })
        .model("ModelA", [&](auto& model) {
            model.lightUnit("ModelLightA", [](auto& lightUnit) {
                lightUnit.light(AmbientLight());
            });
            model.renderUnit("RenderA", [&](auto& renderUnit) {
                renderUnit.mesh(mesh);
            });
        })
        .build();

    REQUIRE(scene != nullptr);

    const auto allLights = scene->getAllLightUnits();
    REQUIRE(allLights.size() == 3);

    CHECK(allLights[0]->getName() == "GlobalLightA");
    CHECK(allLights[1]->getName() == "GlobalLightB");
    CHECK(allLights[2]->getName() == "ModelLightA");

    REQUIRE(scene->getModels().size() == 1);
    const auto model = scene->getModels().front();
    REQUIRE(model != nullptr);
    REQUIRE(model->getRenderUnits().size() == 1);
    CHECK(model->getRenderUnits().front()->getName() == "RenderA");
}
