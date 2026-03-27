#include <doctest.h>
#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/graphics/texturing/cube_map.hpp>

using namespace pixelforge::scene::nodes;
using namespace pixelforge::graphics;


TEST_CASE("Scene has no cube map by default") {
    Scene scene("TestScene");

    CHECK_FALSE(scene.hasCubeMap());
    CHECK(scene.getCubeMap() == nullptr);
}


TEST_CASE("Scene setCubeMap stores provided cube map") {
    Scene scene("TestScene");

    auto cubeMap = std::make_shared<CubeMap>("test");

    scene.setCubeMap(cubeMap);

    CHECK(scene.hasCubeMap());
    CHECK(scene.getCubeMap() == cubeMap);
    CHECK(scene.getCubeMap()->getID() != 0);
}


TEST_CASE("Scene setCubeMap replaces existing cube map") {
    Scene scene("TestScene");

    auto cubeMapA = std::make_shared<CubeMap>("cube_map_a");
    auto cubeMapB = std::make_shared<CubeMap>("cube_map_b");

    scene.setCubeMap(cubeMapA);
    CHECK(scene.getCubeMap() == cubeMapA);

    scene.setCubeMap(cubeMapB);
    CHECK(scene.getCubeMap() == cubeMapB);
}


TEST_CASE("Scene setCubeMap(nullptr) clears cube map") {
    Scene scene("TestScene");

    auto cubeMap = std::make_shared<CubeMap>("test_cube_map");

    scene.setCubeMap(cubeMap);
    REQUIRE(scene.hasCubeMap());

    scene.setCubeMap(std::shared_ptr<CubeMap>(nullptr));

    CHECK_FALSE(scene.hasCubeMap());
    CHECK(scene.getCubeMap() == nullptr);
}
