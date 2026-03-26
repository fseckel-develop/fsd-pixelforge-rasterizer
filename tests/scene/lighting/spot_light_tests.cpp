#include <doctest.h>
#include <pixelforge/scene/lighting/spot_light.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::lighting::Attenuation;
using pixelforge::scene::lighting::Light;
using pixelforge::scene::lighting::SpotLight;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkAttenuationApprox(const Attenuation& actual, const Attenuation& expected) {
        CHECK(actual.constant == doctest::Approx(expected.constant).epsilon(EPS));
        CHECK(actual.linear == doctest::Approx(expected.linear).epsilon(EPS));
        CHECK(actual.quadratic == doctest::Approx(expected.quadratic).epsilon(EPS));
    }

    void checkComponent(const pixelforge::scene::lighting::LightComponent& actual,
                        const glm::vec3& expectedColor,
                        const float expectedIntensity) {
        checkVec3Approx(actual.color, expectedColor);
        CHECK(actual.intensity == doctest::Approx(expectedIntensity).epsilon(EPS));
    }

} // namespace


TEST_CASE("SpotLight default constructor sets type defaults direction and cutoff angles") {
    const SpotLight light;

    CHECK(light.getType() == Light::SPOT);
    checkVec3Approx(light.getPosition(), glm::vec3(0.0f));
    checkVec3Approx(light.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));

    CHECK(light.getInnerCutoff() == doctest::Approx(std::cos(glm::radians(5.0f))).epsilon(EPS));
    CHECK(light.getOuterCutoff() == doctest::Approx(std::cos(glm::radians(15.0f))).epsilon(EPS));

    // Inherited default positional attenuation is R100
    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.045f, 0.0075f});

    checkComponent(light.getAmbient(),  glm::vec3(1.0f), 1.0f);
    checkComponent(light.getDiffuse(),  glm::vec3(1.0f), 1.0f);
    checkComponent(light.getSpecular(), glm::vec3(1.0f), 1.0f);
}

TEST_CASE("SpotLight position and direction constructor stores provided values") {
    const SpotLight light(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(0.0f, -2.0f, 0.0f));

    CHECK(light.getType() == Light::SPOT);
    checkVec3Approx(light.getPosition(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(light.getDirection(), glm::vec3(0.0f, -1.0f, 0.0f));

    CHECK(light.getInnerCutoff() == doctest::Approx(std::cos(glm::radians(5.0f))).epsilon(EPS));
    CHECK(light.getOuterCutoff() == doctest::Approx(std::cos(glm::radians(15.0f))).epsilon(EPS));
}

TEST_CASE("SpotLight setDirection updates normalized direction") {
    SpotLight light;

    light.setDirection(glm::vec3(2.0f, 0.0f, 0.0f));

    checkVec3Approx(light.getDirection(), glm::vec3(1.0f, 0.0f, 0.0f));
}

TEST_CASE("SpotLight setCutoffAngles stores cosines of increasing angles") {
    SpotLight light;

    light.setCutoffAngles(10.0f, 20.0f);

    CHECK(light.getInnerCutoff() == doctest::Approx(std::cos(glm::radians(10.0f))).epsilon(EPS));
    CHECK(light.getOuterCutoff() == doctest::Approx(std::cos(glm::radians(20.0f))).epsilon(EPS));
}

TEST_CASE("SpotLight setCutoffAngles swaps reversed angles") {
    SpotLight light;

    light.setCutoffAngles(25.0f, 10.0f);

    CHECK(light.getInnerCutoff() == doctest::Approx(std::cos(glm::radians(10.0f))).epsilon(EPS));
    CHECK(light.getOuterCutoff() == doctest::Approx(std::cos(glm::radians(25.0f))).epsilon(EPS));
}

TEST_CASE("SpotLight preserves position and attenuation when changing direction and cutoffs") {
    SpotLight light(glm::vec3(4.0f, 5.0f, 6.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    light.setRange(pixelforge::scene::lighting::R20);
    light.setDirection(glm::vec3(0.0f, 0.0f, -3.0f));
    light.setCutoffAngles(12.0f, 24.0f);

    checkVec3Approx(light.getPosition(), glm::vec3(4.0f, 5.0f, 6.0f));
    checkVec3Approx(light.getDirection(), glm::vec3(0.0f, 0.0f, -1.0f));
    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.22f, 0.20f});

    CHECK(light.getInnerCutoff() == doctest::Approx(std::cos(glm::radians(12.0f))).epsilon(EPS));
    CHECK(light.getOuterCutoff() == doctest::Approx(std::cos(glm::radians(24.0f))).epsilon(EPS));
}

TEST_CASE("SpotLight inherited setters for position attenuation and light components still work") {
    SpotLight light;

    light.setPosition(glm::vec3(-1.0f, 2.0f, -3.0f));
    light.setAttenuation(Attenuation{1.0f, 0.14f, 0.07f});
    light.setColor(glm::vec3(0.25f, 0.5f, 0.75f));
    light.setIntensity(0.6f);

    checkVec3Approx(light.getPosition(), glm::vec3(-1.0f, 2.0f, -3.0f));
    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.14f, 0.07f});

    checkComponent(light.getAmbient(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
    checkComponent(light.getSpecular(), glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
}
