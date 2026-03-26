#include <doctest.h>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::lighting::DirectionalLight;
using pixelforge::scene::lighting::Light;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkComponent(const pixelforge::scene::lighting::LightComponent& actual,
                        const glm::vec3& expectedColor,
                        const float expectedIntensity) {
        checkVec3Approx(actual.color, expectedColor);
        CHECK(actual.intensity == doctest::Approx(expectedIntensity).epsilon(EPS));
    }

} // namespace


TEST_CASE("DirectionalLight default constructor sets type and default direction") {
    const DirectionalLight light;

    CHECK(light.getType() == Light::DIRECTIONAL);
    checkVec3Approx(light.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));

    checkComponent(light.getAmbient(),  glm::vec3(1.0f), 1.0f);
    checkComponent(light.getDiffuse(),  glm::vec3(1.0f), 1.0f);
    checkComponent(light.getSpecular(), glm::vec3(1.0f), 1.0f);

    checkVec3Approx(light.getPosition(), glm::vec3(0.0f));
}

TEST_CASE("DirectionalLight custom constructor stores normalized direction") {
    const DirectionalLight light(glm::vec3(0.0f, -2.0f, 0.0f));

    CHECK(light.getType() == Light::DIRECTIONAL);
    checkVec3Approx(light.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));
}

TEST_CASE("DirectionalLight setDirection updates normalized direction") {
    DirectionalLight light;

    light.setDirection(glm::vec3(2.0f, 0.0f, 0.0f));

    checkVec3Approx(light.getDirection(), glm::vec3(1.0f, 0.0f, 0.0f));
}

TEST_CASE("DirectionalLight preserves light components when changing direction") {
    DirectionalLight light;

    light.setAmbient(glm::vec3(0.2f, 0.3f, 0.4f), 0.5f);
    light.setDiffuse(glm::vec3(0.5f, 0.6f, 0.7f), 0.8f);
    light.setSpecular(glm::vec3(0.7f, 0.8f, 0.9f), 0.4f);

    light.setDirection(glm::vec3(0.0f, 0.0f, -3.0f));

    checkVec3Approx(light.getDirection(), glm::vec3(0.0f, 0.0f, -1.0f));
    checkComponent(light.getAmbient(),  glm::vec3(0.2f, 0.3f, 0.4f), 0.5f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.5f, 0.6f, 0.7f), 0.8f);
    checkComponent(light.getSpecular(), glm::vec3(0.7f, 0.8f, 0.9f), 0.4f);
}

TEST_CASE("DirectionalLight inherited setters still work") {
    DirectionalLight light;

    light.setColor(glm::vec3(0.25f, 0.5f, 0.75f));
    light.setIntensity(0.6f);

    checkComponent(light.getAmbient(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
    checkComponent(light.getSpecular(), glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
}

TEST_CASE("DirectionalLight default position remains zero") {
    const DirectionalLight light(glm::vec3(1.0f, 2.0f, 3.0f));

    checkVec3Approx(light.getPosition(), glm::vec3(0.0f));
}
