#include <doctest.h>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <memory>

using pixelforge::geometry::Cuboid;
using pixelforge::scene::lighting::DirectionalLight;
using pixelforge::scene::lighting::PositionalLight;
using pixelforge::scene::nodes::LightUnit;
using pixelforge::scene::nodes::Model;
using pixelforge::scene::nodes::RenderUnit;
using pixelforge::scene::nodes::TransformNode;


TEST_CASE("Model default constructor starts with no light units render units or children") {
    const auto model = std::make_shared<Model>("Model");

    CHECK(model->getLightUnits().empty());
    CHECK(model->getRenderUnits().empty());
    CHECK(model->getChildren().empty());
}

TEST_CASE("Model addLightUnit stores light unit and adds it as child") {
    const auto model = std::make_shared<Model>("Model");
    const auto lightUnit = std::make_shared<LightUnit>(
        "KeyLight",
        std::make_shared<DirectionalLight>()
    );

    model->addLightUnit(lightUnit);

    REQUIRE(model->getLightUnits().size() == 1);
    CHECK(model->getLightUnits()[0] == lightUnit);

    REQUIRE(model->getChildren().size() == 1);
    CHECK(model->getChildren()[0] == lightUnit);

    CHECK(lightUnit->getParent() == model);
}

TEST_CASE("Model addRenderUnit stores render unit and adds it as child") {
    const auto model = std::make_shared<Model>("Model");
    const auto renderUnit = std::make_shared<RenderUnit>(
        "Body",
        std::make_shared<Cuboid>(1.0f, 2.0f, 3.0f)
    );

    model->addRenderUnit(renderUnit);

    REQUIRE(model->getRenderUnits().size() == 1);
    CHECK(model->getRenderUnits()[0] == renderUnit);

    REQUIRE(model->getChildren().size() == 1);
    CHECK(model->getChildren()[0] == renderUnit);

    CHECK(renderUnit->getParent() == model);
}

TEST_CASE("Model getLightUnitByName returns matching light unit") {
    const auto model = std::make_shared<Model>("Model");
    const auto lightA = std::make_shared<LightUnit>(
        "KeyLight",
        std::make_shared<DirectionalLight>()
    );
    const auto lightB = std::make_shared<LightUnit>(
        "FillLight",
        std::make_shared<PositionalLight>()
    );

    model->addLightUnit(lightA);
    model->addLightUnit(lightB);

    CHECK(model->getLightUnitByName("KeyLight") == lightA);
    CHECK(model->getLightUnitByName("FillLight") == lightB);
}

TEST_CASE("Model getRenderUnitByName returns matching render unit") {
    const auto model = std::make_shared<Model>("Model");
    const auto renderA = std::make_shared<RenderUnit>(
        "Body",
        std::make_shared<Cuboid>(1.0f, 2.0f, 3.0f)
    );
    const auto renderB = std::make_shared<RenderUnit>(
        "Wheels",
        std::make_shared<Cuboid>(4.0f, 5.0f, 6.0f)
    );

    model->addRenderUnit(renderA);
    model->addRenderUnit(renderB);

    CHECK(model->getRenderUnitByName("Body") == renderA);
    CHECK(model->getRenderUnitByName("Wheels") == renderB);
}

TEST_CASE("Model lookup by missing name returns nullptr") {
    const auto model = std::make_shared<Model>("Model");

    CHECK(model->getLightUnitByName("MissingLight") == nullptr);
    CHECK(model->getRenderUnitByName("MissingRenderUnit") == nullptr);
}

TEST_CASE("Model can store multiple light units and render units independently") {
    const auto model = std::make_shared<Model>("Model");

    const auto lightA = std::make_shared<LightUnit>(
        "LightA",
        std::make_shared<DirectionalLight>()
    );
    const auto lightB = std::make_shared<LightUnit>(
        "LightB",
        std::make_shared<PositionalLight>()
    );

    const auto renderA = std::make_shared<RenderUnit>(
        "RenderA",
        std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f)
    );
    const auto renderB = std::make_shared<RenderUnit>(
        "RenderB",
        std::make_shared<Cuboid>(2.0f, 2.0f, 2.0f)
    );

    model->addLightUnit(lightA);
    model->addLightUnit(lightB);
    model->addRenderUnit(renderA);
    model->addRenderUnit(renderB);

    CHECK(model->getLightUnits().size() == 2);
    CHECK(model->getRenderUnits().size() == 2);
    CHECK(model->getChildren().size() == 4);
}

TEST_CASE("Model adding unit with existing transform root adds that root as child") {
    const auto model = std::make_shared<Model>("Model");

    const auto root = std::make_shared<TransformNode>("Root");
    const auto renderUnit = std::make_shared<RenderUnit>(
        "Render",
        std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f)
    );

    root->addChild(renderUnit);

    REQUIRE(renderUnit->getRoot() == root);

    model->addRenderUnit(renderUnit);

    REQUIRE(model->getRenderUnits().size() == 1);
    CHECK(model->getRenderUnits()[0] == renderUnit);

    REQUIRE(model->getChildren().size() == 1);
    CHECK(model->getChildren()[0] == root);
    CHECK(root->getParent() == model);
}

TEST_CASE("Model adding light unit with existing transform root adds that root as child") {
    const auto model = std::make_shared<Model>("Model");

    const auto root = std::make_shared<TransformNode>("Root");
    const auto lightUnit = std::make_shared<LightUnit>(
        "Light",
        std::make_shared<DirectionalLight>()
    );

    root->addChild(lightUnit);

    REQUIRE(lightUnit->getRoot() == root);

    model->addLightUnit(lightUnit);

    REQUIRE(model->getLightUnits().size() == 1);
    CHECK(model->getLightUnits()[0] == lightUnit);

    REQUIRE(model->getChildren().size() == 1);
    CHECK(model->getChildren()[0] == root);
    CHECK(root->getParent() == model);
}
