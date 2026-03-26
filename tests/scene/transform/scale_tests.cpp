#include <doctest.h>
#include <pixelforge/scene/transform/scale.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::transform::Scale;
using pixelforge::scene::transform::Transform;


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


TEST_CASE("Scale default constructor produces identity transform") {
    const Transform transform = Scale();

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Scale uniform constructor sets equal scale on all axes") {
    const Transform transform = Scale(2.5f);

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(2.5f, 2.5f, 2.5f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Scale vector constructor sets scale components") {
    const Transform transform = Scale(glm::vec3(2.0f, 3.0f, 4.0f));

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Scale scalar constructor with components sets scale per axis") {
    const Transform transform = Scale(5.0f, 6.0f, 7.0f);

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(5.0f, 6.0f, 7.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Scale toMatrix scales points correctly") {
    const Transform transform = Scale(2.0f, 3.0f, 4.0f);

    const glm::vec4 point(1.0f, 2.0f, 3.0f, 1.0f);
    const glm::vec4 transformed = transform.toMatrix() * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(2.0f, 6.0f, 12.0f));
}
