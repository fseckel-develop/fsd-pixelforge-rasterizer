#include <doctest.h>
#include <pixelforge/scene/lighting/light.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::lighting::Light;
using pixelforge::scene::lighting::LightComponent;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkLightComponent(const LightComponent& actual,
                             const glm::vec3& expectedColor,
                             const float expectedIntensity) {
        checkVec3Approx(actual.color, expectedColor);
        CHECK(actual.intensity == doctest::Approx(expectedIntensity).epsilon(EPS));
    }

} // namespace


TEST_CASE("Light default constructor initializes white components with full intensity") {
    const Light light;

    CHECK(light.getType() == Light::AMBIENT);

    checkLightComponent(light.getAmbient(),  glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    checkLightComponent(light.getDiffuse(),  glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    checkLightComponent(light.getSpecular(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

    checkVec3Approx(light.getDirection(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(light.getPosition(),  glm::vec3(0.0f, 0.0f, 0.0f));
}

TEST_CASE("Light setColor updates all components and clamps values to [0, 1]") {
    Light light;

    light.setColor(glm::vec3(-1.0f, 0.5f, 2.0f));

    const glm::vec3 expectedColor(0.0f, 0.5f, 1.0f);

    checkVec3Approx(light.getAmbient().color, expectedColor);
    checkVec3Approx(light.getDiffuse().color, expectedColor);
    checkVec3Approx(light.getSpecular().color, expectedColor);

    CHECK(light.getAmbient().intensity == doctest::Approx(1.0f));
    CHECK(light.getDiffuse().intensity == doctest::Approx(1.0f));
    CHECK(light.getSpecular().intensity == doctest::Approx(1.0f));
}

TEST_CASE("Light setIntensity updates all components and clamps value to [0, 1]") {
    Light light;

    light.setIntensity(1.5f);

    CHECK(light.getAmbient().intensity == doctest::Approx(1.0f));
    CHECK(light.getDiffuse().intensity == doctest::Approx(1.0f));
    CHECK(light.getSpecular().intensity == doctest::Approx(1.0f));

    light.setIntensity(-0.25f);

    CHECK(light.getAmbient().intensity == doctest::Approx(0.0f));
    CHECK(light.getDiffuse().intensity == doctest::Approx(0.0f));
    CHECK(light.getSpecular().intensity == doctest::Approx(0.0f));
}

TEST_CASE("Light setAmbient only updates ambient component") {
    Light light;

    light.setAmbient(glm::vec3(0.2f, 0.4f, 0.6f), 0.8f);

    checkLightComponent(light.getAmbient(), glm::vec3(0.2f, 0.4f, 0.6f), 0.8f);
    checkLightComponent(light.getDiffuse(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    checkLightComponent(light.getSpecular(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
}

TEST_CASE("Light setDiffuse only updates diffuse component") {
    Light light;

    light.setDiffuse(glm::vec3(0.3f, 0.5f, 0.7f), 0.4f);

    checkLightComponent(light.getAmbient(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    checkLightComponent(light.getDiffuse(), glm::vec3(0.3f, 0.5f, 0.7f), 0.4f);
    checkLightComponent(light.getSpecular(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
}

TEST_CASE("Light setSpecular only updates specular component") {
    Light light;

    light.setSpecular(glm::vec3(0.9f, 0.8f, 0.7f), 0.6f);

    checkLightComponent(light.getAmbient(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    checkLightComponent(light.getDiffuse(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    checkLightComponent(light.getSpecular(), glm::vec3(0.9f, 0.8f, 0.7f), 0.6f);
}

TEST_CASE("Light component setters clamp color and intensity values") {
    Light light;

    light.setAmbient(glm::vec3(-1.0f, 0.25f, 2.0f), 1.5f);
    light.setDiffuse(glm::vec3(5.0f, -2.0f, 0.5f), -0.5f);
    light.setSpecular(glm::vec3(0.1f, 3.0f, -4.0f), 0.75f);

    checkLightComponent(light.getAmbient(),  glm::vec3(0.0f, 0.25f, 1.0f), 1.0f);
    checkLightComponent(light.getDiffuse(),  glm::vec3(1.0f, 0.0f, 0.5f), 0.0f);
    checkLightComponent(light.getSpecular(), glm::vec3(0.1f, 1.0f, 0.0f), 0.75f);
}

TEST_CASE("Light setColor preserves existing intensities") {
    Light light;

    light.setAmbient(glm::vec3(1.0f), 0.2f);
    light.setDiffuse(glm::vec3(1.0f), 0.4f);
    light.setSpecular(glm::vec3(1.0f), 0.6f);

    light.setColor(glm::vec3(0.25f, 0.5f, 0.75f));

    checkLightComponent(light.getAmbient(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.2f);
    checkLightComponent(light.getDiffuse(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.4f);
    checkLightComponent(light.getSpecular(), glm::vec3(0.25f, 0.5f, 0.75f), 0.6f);
}

TEST_CASE("Light setIntensity preserves existing colors") {
    Light light;

    light.setAmbient(glm::vec3(0.1f, 0.2f, 0.3f), 1.0f);
    light.setDiffuse(glm::vec3(0.4f, 0.5f, 0.6f), 1.0f);
    light.setSpecular(glm::vec3(0.7f, 0.8f, 0.9f), 1.0f);

    light.setIntensity(0.35f);

    checkLightComponent(light.getAmbient(),  glm::vec3(0.1f, 0.2f, 0.3f), 0.35f);
    checkLightComponent(light.getDiffuse(),  glm::vec3(0.4f, 0.5f, 0.6f), 0.35f);
    checkLightComponent(light.getSpecular(), glm::vec3(0.7f, 0.8f, 0.9f), 0.35f);
}
