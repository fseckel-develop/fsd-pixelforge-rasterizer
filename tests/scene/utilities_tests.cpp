#include <doctest.h>
#include <pixelforge/scene/utilities.hpp>
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/geometry/meshes/cuboid.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <stdexcept>

using pixelforge::scene::nodes::LightUnit;
using pixelforge::scene::nodes::Model;
using pixelforge::scene::nodes::RenderUnit;
using pixelforge::scene::nodes::Scene;
using pixelforge::scene::utilities::printSceneNodeTree;
using pixelforge::scene::utilities::printSceneSummary;
using pixelforge::scene::utilities::validateDirection;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    class CoutRedirect {
    public:
        CoutRedirect() :
            old_(std::cout.rdbuf(stream_.rdbuf())) {
        }

        ~CoutRedirect() {
            std::cout.rdbuf(old_);
        }

        [[nodiscard]] std::string str() const {
            return stream_.str();
        }

    private:
        std::ostringstream stream_;
        std::streambuf* old_;
    };

} // namespace


TEST_CASE("validateDirection normalizes non-zero direction") {
    const glm::vec3 result = validateDirection(glm::vec3(3.0f, 0.0f, 0.0f), "test");

    checkVec3Approx(result, glm::vec3(1.0f, 0.0f, 0.0f));
}

TEST_CASE("validateDirection preserves already normalized direction") {
    const glm::vec3 input(0.0f, 1.0f, 0.0f);
    const glm::vec3 result = validateDirection(input, "test");

    checkVec3Approx(result, input);
}

TEST_CASE("validateDirection throws for zero vector") {
    CHECK_THROWS_AS(validateDirection(glm::vec3(0.0f), "test"), std::invalid_argument);
}

TEST_CASE("validateDirection error message includes context when provided") {
    try {
        static_cast<void>(validateDirection(glm::vec3(0.0f), "MyContext"));
        FAIL("Expected exception was not thrown");
    } catch (const std::invalid_argument& ex) {
        CHECK(std::string(ex.what()).find("MyContext") != std::string::npos);
    }
}

TEST_CASE("validateDirection error message omits context suffix when empty") {
    try {
        static_cast<void>(validateDirection(glm::vec3(0.0f), ""));
        FAIL("Expected exception was not thrown");
    } catch (const std::invalid_argument& ex) {
        CHECK(std::string(ex.what()).find("Invalid zero direction vector") != std::string::npos);
    }
}

TEST_CASE("printSceneNodeTree prints null placeholder for null node") {
    CoutRedirect capture;

    printSceneNodeTree(nullptr);

    const std::string output = capture.str();
    CHECK(output.find("=== Scene Node Tree ===") != std::string::npos);
    CHECK(output.find("<null>") != std::string::npos);
    CHECK(output.find("=== End Scene Node Tree ===") != std::string::npos);
}

TEST_CASE("printSceneSummary prints null message for null scene") {
    CoutRedirect capture;

    printSceneSummary(nullptr);

    CHECK(capture.str().find("Scene is null") != std::string::npos);
}

TEST_CASE("printSceneNodeTree prints node hierarchy with type names") {
    const auto scene = std::make_shared<Scene>("TestScene");
    const auto model = std::make_shared<Model>("TestModel");
    const auto renderUnit = std::make_shared<RenderUnit>(
        "TestRenderUnit",
        std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f)
    );
    const auto lightUnit = std::make_shared<LightUnit>(
        "TestLightUnit",
        std::make_shared<pixelforge::scene::lighting::DirectionalLight>()
    );

    model->addRenderUnit(renderUnit);
    scene->addModel(model);
    scene->addLightUnit(lightUnit);

    CoutRedirect capture;
    printSceneNodeTree(scene);

    const std::string output = capture.str();
    CHECK(output.find("Scene \"TestScene\"") != std::string::npos);
    CHECK(output.find("Model \"TestModel\"") != std::string::npos);
    CHECK(output.find("RenderUnit \"TestRenderUnit\"") != std::string::npos);
    CHECK(output.find("LightUnit \"TestLightUnit\"") != std::string::npos);
    CHECK(output.find("[children: ") != std::string::npos);
}

TEST_CASE("printSceneSummary prints scene model and light counts") {
    const auto scene = std::make_shared<Scene>("SceneA");
    const auto model = std::make_shared<Model>("ModelA");
    const auto renderUnit = std::make_shared<RenderUnit>(
        "RenderA",
        std::make_shared<pixelforge::geometry::Cuboid>(1.0f, 1.0f, 1.0f)
    );
    const auto modelLight = std::make_shared<LightUnit>(
        "ModelLight",
        std::make_shared<pixelforge::scene::lighting::DirectionalLight>()
    );
    const auto globalLight = std::make_shared<LightUnit>(
        "GlobalLight",
        std::make_shared<pixelforge::scene::lighting::DirectionalLight>()
    );

    model->addRenderUnit(renderUnit);
    model->addLightUnit(modelLight);
    scene->addModel(model);
    scene->addLightUnit(globalLight);

    CoutRedirect capture;
    printSceneSummary(scene);

    const std::string output = capture.str();
    CHECK(output.find("=== Scene Summary ===") != std::string::npos);
    CHECK(output.find("Scene: SceneA") != std::string::npos);
    CHECK(output.find("Models: 1") != std::string::npos);
    CHECK(output.find("All LightUnits: 2") != std::string::npos);
    CHECK(output.find("Model \"ModelA\" summary:") != std::string::npos);
    CHECK(output.find("renderUnits: 1") != std::string::npos);
    CHECK(output.find("lightUnits: 1") != std::string::npos);
    CHECK(output.find("root: ") != std::string::npos);
    CHECK(output.find("=== End Scene Summary ===") != std::string::npos);
}
