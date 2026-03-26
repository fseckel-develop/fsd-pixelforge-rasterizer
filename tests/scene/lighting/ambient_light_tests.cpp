#include <doctest.h>
#include <pixelforge/scene/lighting/ambient_light.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::lighting::AmbientLight;
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


TEST_CASE("AmbientLight default constructor creates ambient-only light") {
    const AmbientLight light;

    CHECK(light.getType() == Light::AMBIENT);

    checkComponent(light.getAmbient(),  glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    checkComponent(light.getSpecular(), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
}

TEST_CASE("AmbientLight custom constructor stores provided ambient values") {
    const AmbientLight light(glm::vec3(0.2f, 0.4f, 0.6f), 0.75f);

    CHECK(light.getType() == Light::AMBIENT);

    checkComponent(light.getAmbient(),  glm::vec3(0.2f, 0.4f, 0.6f), 0.75f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    checkComponent(light.getSpecular(), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
}

TEST_CASE("AmbientLight setAmbient updates only ambient component") {
    AmbientLight light;

    light.setAmbient(glm::vec3(0.3f, 0.5f, 0.7f), 0.8f);

    checkComponent(light.getAmbient(),  glm::vec3(0.3f, 0.5f, 0.7f), 0.8f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    checkComponent(light.getSpecular(), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
}

TEST_CASE("AmbientLight setDiffuse has no effect") {
    AmbientLight light;

    const auto beforeAmbient = light.getAmbient();
    const auto beforeDiffuse = light.getDiffuse();
    const auto beforeSpecular = light.getSpecular();

    light.setDiffuse(glm::vec3(1.0f, 0.5f, 0.25f), 0.9f);

    checkComponent(light.getAmbient(),  beforeAmbient.color, beforeAmbient.intensity);
    checkComponent(light.getDiffuse(),  beforeDiffuse.color, beforeDiffuse.intensity);
    checkComponent(light.getSpecular(), beforeSpecular.color, beforeSpecular.intensity);
}

TEST_CASE("AmbientLight setSpecular has no effect") {
    AmbientLight light;

    const auto beforeAmbient = light.getAmbient();
    const auto beforeDiffuse = light.getDiffuse();
    const auto beforeSpecular = light.getSpecular();

    light.setSpecular(glm::vec3(1.0f, 0.5f, 0.25f), 0.9f);

    checkComponent(light.getAmbient(),  beforeAmbient.color, beforeAmbient.intensity);
    checkComponent(light.getDiffuse(),  beforeDiffuse.color, beforeDiffuse.intensity);
    checkComponent(light.getSpecular(), beforeSpecular.color, beforeSpecular.intensity);
}

TEST_CASE("AmbientLight inherited setColor updates ambient and zeroed components' colors") {
    AmbientLight light;

    light.setColor(glm::vec3(0.25f, 0.5f, 0.75f));

    checkComponent(light.getAmbient(),  glm::vec3(0.25f, 0.5f, 0.75f), 1.0f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.25f, 0.5f, 0.75f), 0.0f);
    checkComponent(light.getSpecular(), glm::vec3(0.25f, 0.5f, 0.75f), 0.0f);
}

TEST_CASE("AmbientLight inherited setIntensity updates ambient and zeroed components' intensities") {
    AmbientLight light;

    light.setIntensity(0.4f);

    checkComponent(light.getAmbient(),  glm::vec3(1.0f, 1.0f, 1.0f), 0.4f);
    checkComponent(light.getDiffuse(),  glm::vec3(0.0f, 0.0f, 0.0f), 0.4f);
    checkComponent(light.getSpecular(), glm::vec3(0.0f, 0.0f, 0.0f), 0.4f);
}
