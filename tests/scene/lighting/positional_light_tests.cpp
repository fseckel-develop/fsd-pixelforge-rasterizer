#include <doctest.h>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::lighting::Attenuation;
using pixelforge::scene::lighting::Light;
using pixelforge::scene::lighting::PositionalLight;


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


TEST_CASE("PositionalLight default constructor sets type origin position and default attenuation") {
    const PositionalLight light;

    CHECK(light.getType() == Light::POSITIONAL);
    checkVec3Approx(light.getPosition(), glm::vec3(0.0f, 0.0f, 0.0f));

    // Default is R100
    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.045f, 0.0075f});

    checkComponent(light.getAmbient(),  glm::vec3(1.0f), 1.0f);
    checkComponent(light.getDiffuse(),  glm::vec3(1.0f), 1.0f);
    checkComponent(light.getSpecular(), glm::vec3(1.0f), 1.0f);

    checkVec3Approx(light.getDirection(), glm::vec3(0.0f));
}

TEST_CASE("PositionalLight position constructor stores provided position") {
    const PositionalLight light(glm::vec3(1.0f, 2.0f, 3.0f));

    CHECK(light.getType() == Light::POSITIONAL);
    checkVec3Approx(light.getPosition(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.045f, 0.0075f});
}

TEST_CASE("PositionalLight setPosition updates position only") {
    PositionalLight light;

    light.setAmbient(glm::vec3(0.2f, 0.3f, 0.4f), 0.5f);
    light.setDiffuse(glm::vec3(0.5f, 0.6f, 0.7f), 0.8f);
    light.setSpecular(glm::vec3(0.7f, 0.8f, 0.9f), 0.4f);

    light.setPosition(glm::vec3(-4.0f, 5.0f, -6.0f));

    checkVec3Approx(light.getPosition(), glm::vec3(-4.0f, 5.0f, -6.0f));
    checkComponent(light.getAmbient(),  glm::vec3(0.2f, 0.3f, 0.4f), 0.5f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.5f, 0.6f, 0.7f), 0.8f);
    checkComponent(light.getSpecular(), glm::vec3(0.7f, 0.8f, 0.9f), 0.4f);
}

TEST_CASE("PositionalLight setAttenuation updates attenuation exactly") {
    PositionalLight light;

    const Attenuation attenuation{1.0f, 0.22f, 0.20f};
    light.setAttenuation(attenuation);

    checkAttenuationApprox(light.getAttenuation(), attenuation);
}

TEST_CASE("PositionalLight setRange R3250 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R3250);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.0014f, 0.000007f});
}

TEST_CASE("PositionalLight setRange R600 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R600);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.007f, 0.0002f});
}

TEST_CASE("PositionalLight setRange R325 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R325);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.014f, 0.0007f});
}

TEST_CASE("PositionalLight setRange R200 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R200);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.022f, 0.0019f});
}

TEST_CASE("PositionalLight setRange R160 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R160);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.027f, 0.0028f});
}

TEST_CASE("PositionalLight setRange R100 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R100);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.045f, 0.0075f});
}

TEST_CASE("PositionalLight setRange R65 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R65);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.07f, 0.017f});
}

TEST_CASE("PositionalLight setRange R50 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R50);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.09f, 0.032f});
}

TEST_CASE("PositionalLight setRange R32 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R32);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.14f, 0.07f});
}

TEST_CASE("PositionalLight setRange R20 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R20);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.22f, 0.20f});
}

TEST_CASE("PositionalLight setRange R13 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R13);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.35f, 0.44f});
}

TEST_CASE("PositionalLight setRange R7 sets expected attenuation") {
    PositionalLight light;
    light.setRange(pixelforge::scene::lighting::R7);

    checkAttenuationApprox(light.getAttenuation(), Attenuation{1.0f, 0.7f, 1.8f});
}

TEST_CASE("PositionalLight inherited light component setters still work") {
    PositionalLight light;

    light.setColor(glm::vec3(0.25f, 0.5f, 0.75f));
    light.setIntensity(0.6f);

    checkComponent(light.getAmbient(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
    checkComponent(light.getSpecular(), glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
}
