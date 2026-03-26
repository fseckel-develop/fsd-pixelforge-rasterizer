#include <doctest.h>
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/nodes/transform_node.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <memory>

using pixelforge::geometry::Cuboid;
using pixelforge::scene::lighting::DirectionalLight;
using pixelforge::scene::lighting::PositionalLight;
using pixelforge::scene::nodes::LightUnit;
using pixelforge::scene::nodes::Model;
using pixelforge::scene::nodes::Scene;
using pixelforge::scene::nodes::TransformNode;


TEST_CASE("Scene default constructor starts with no models no global lights and no children") {
    const auto scene = std::make_shared<Scene>("Scene");

    CHECK(scene->getModels().empty());
    CHECK(scene->getGlobalLightUnits().empty());
    CHECK(scene->getAllLightUnits().empty());
    CHECK(scene->getChildren().empty());
}

TEST_CASE("Scene addModel stores model and adds it as child") {
    const auto scene = std::make_shared<Scene>("Scene");
    const auto model = std::make_shared<Model>("Model");

    scene->addModel(model);

    REQUIRE(scene->getModels().size() == 1);
    CHECK(scene->getModels()[0] == model);

    REQUIRE(scene->getChildren().size() == 1);
    CHECK(scene->getChildren()[0] == model);

    CHECK(model->getParent() == scene);
}

TEST_CASE("Scene addLightUnit stores global light and adds it as child") {
    const auto scene = std::make_shared<Scene>("Scene");
    const auto lightUnit = std::make_shared<LightUnit>(
        "GlobalLight",
        std::make_shared<DirectionalLight>()
    );

    scene->addLightUnit(lightUnit);

    REQUIRE(scene->getGlobalLightUnits().size() == 1);
    CHECK(scene->getGlobalLightUnits()[0] == lightUnit);

    REQUIRE(scene->getChildren().size() == 1);
    CHECK(scene->getChildren()[0] == lightUnit);

    CHECK(lightUnit->getParent() == scene);
}

TEST_CASE("Scene ignores null model") {
    const auto scene = std::make_shared<Scene>("Scene");
    std::shared_ptr<Model> model = nullptr;

    scene->addModel(model);

    CHECK(scene->getModels().empty());
    CHECK(scene->getChildren().empty());
}

TEST_CASE("Scene ignores null global light unit") {
    const auto scene = std::make_shared<Scene>("Scene");
    std::shared_ptr<LightUnit> lightUnit = nullptr;

    scene->addLightUnit(lightUnit);

    CHECK(scene->getGlobalLightUnits().empty());
    CHECK(scene->getChildren().empty());
}

TEST_CASE("Scene getAllLightUnits returns global and model light units") {
    const auto scene = std::make_shared<Scene>("Scene");

    const auto globalLight = std::make_shared<LightUnit>(
        "GlobalLight",
        std::make_shared<DirectionalLight>()
    );

    const auto modelA = std::make_shared<Model>("ModelA");
    const auto modelB = std::make_shared<Model>("ModelB");

    const auto modelLightA = std::make_shared<LightUnit>(
        "ModelLightA",
        std::make_shared<PositionalLight>()
    );
    const auto modelLightB = std::make_shared<LightUnit>(
        "ModelLightB",
        std::make_shared<DirectionalLight>()
    );

    modelA->addLightUnit(modelLightA);
    modelB->addLightUnit(modelLightB);

    scene->addLightUnit(globalLight);
    scene->addModel(modelA);
    scene->addModel(modelB);

    const auto allLights = scene->getAllLightUnits();

    REQUIRE(allLights.size() == 3);
    CHECK(allLights[0] == globalLight);
    CHECK(allLights[1] == modelLightA);
    CHECK(allLights[2] == modelLightB);
}

TEST_CASE("Scene getAllLightUnits returns only global lights when no models exist") {
    const auto scene = std::make_shared<Scene>("Scene");

    const auto lightA = std::make_shared<LightUnit>(
        "LightA",
        std::make_shared<DirectionalLight>()
    );
    const auto lightB = std::make_shared<LightUnit>(
        "LightB",
        std::make_shared<PositionalLight>()
    );

    scene->addLightUnit(lightA);
    scene->addLightUnit(lightB);

    const auto allLights = scene->getAllLightUnits();

    REQUIRE(allLights.size() == 2);
    CHECK(allLights[0] == lightA);
    CHECK(allLights[1] == lightB);
}

TEST_CASE("Scene getAllLightUnits returns only model lights when no global lights exist") {
    const auto scene = std::make_shared<Scene>("Scene");
    const auto model = std::make_shared<Model>("Model");

    const auto lightA = std::make_shared<LightUnit>(
        "LightA",
        std::make_shared<DirectionalLight>()
    );
    const auto lightB = std::make_shared<LightUnit>(
        "LightB",
        std::make_shared<PositionalLight>()
    );

    model->addLightUnit(lightA);
    model->addLightUnit(lightB);
    scene->addModel(model);

    const auto allLights = scene->getAllLightUnits();

    REQUIRE(allLights.size() == 2);
    CHECK(allLights[0] == lightA);
    CHECK(allLights[1] == lightB);
}

TEST_CASE("Scene adding model with existing root adds that root as child") {
    const auto scene = std::make_shared<Scene>("Scene");

    const auto root = std::make_shared<TransformNode>("Root");
    const auto model = std::make_shared<Model>("Model");

    root->addChild(model);
    REQUIRE(model->getRoot() == root);

    scene->addModel(model);

    REQUIRE(scene->getModels().size() == 1);
    CHECK(scene->getModels()[0] == model);

    REQUIRE(scene->getChildren().size() == 1);
    CHECK(scene->getChildren()[0] == root);
    CHECK(root->getParent() == scene);
}

TEST_CASE("Scene adding light unit with existing root adds that root as child") {
    const auto scene = std::make_shared<Scene>("Scene");

    const auto root = std::make_shared<TransformNode>("Root");
    const auto lightUnit = std::make_shared<LightUnit>(
        "GlobalLight",
        std::make_shared<DirectionalLight>()
    );

    root->addChild(lightUnit);
    REQUIRE(lightUnit->getRoot() == root);

    scene->addLightUnit(lightUnit);

    REQUIRE(scene->getGlobalLightUnits().size() == 1);
    CHECK(scene->getGlobalLightUnits()[0] == lightUnit);

    REQUIRE(scene->getChildren().size() == 1);
    CHECK(scene->getChildren()[0] == root);
    CHECK(root->getParent() == scene);
}
