#include <doctest.h>
#include <pixelforge/scene/transform/translate.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::transform::Transform;
using pixelforge::scene::transform::Translate;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    void checkQuatApprox(const glm::quat& actual, const glm::quat& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
        CHECK(actual.w == doctest::Approx(expected.w).epsilon(EPS));
    }

} // namespace


TEST_CASE("Translate default constructor produces identity transform") {
    const Transform transform = Translate();

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Translate vector constructor sets translation") {
    const Transform transform = Translate(glm::vec3(1.0f, 2.0f, 3.0f));

    checkVec3Approx(transform.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Translate scalar constructor sets translation") {
    const Transform transform = Translate(4.0f, 5.0f, 6.0f);

    checkVec3Approx(transform.getTranslation(), glm::vec3(4.0f, 5.0f, 6.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Translate toMatrix translates points correctly") {
    const Transform transform = Translate(3.0f, -2.0f, 5.0f);

    constexpr glm::vec4 point(1.0f, 2.0f, 3.0f, 1.0f);
    const glm::vec4 transformed = transform.toMatrix() * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(4.0f, 0.0f, 8.0f));
}
