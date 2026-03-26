#include <doctest.h>
#include <pixelforge/scene/transform/rotate.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::transform::Rotate;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

    void checkQuatApprox(const glm::quat& actual, const glm::quat& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
        CHECK(actual.w == doctest::Approx(expected.w).epsilon(eps));
    }

} // namespace


TEST_CASE("Rotate default constructor produces identity transform") {
    const Transform transform = Rotate();

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Rotate quaternion constructor sets rotation") {
    const glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));

    const Transform transform = Rotate(q);

    checkQuatApprox(transform.getRotation(), q, 1e-4f);
    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f));
}

TEST_CASE("Rotate Euler vector constructor sets rotation (degrees)") {
    const Transform transform = Rotate(glm::vec3(0.0f, 0.0f, 90.0f));

    const glm::quat expected = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 90.0f)));

    checkQuatApprox(transform.getRotation(), expected, 1e-4f);
}

TEST_CASE("Rotate scalar constructor sets rotation (degrees)") {
    const Transform transform = Rotate(0.0f, 90.0f, 0.0f);

    const glm::quat expected = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));

    checkQuatApprox(transform.getRotation(), expected, 1e-4f);
}

TEST_CASE("Rotate toMatrix rotates a point around Z axis") {
    const Transform transform = Rotate(0.0f, 0.0f, 90.0f);

    const glm::vec4 point(1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 transformed = transform.toMatrix() * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(0.0f, 1.0f, 0.0f), 1e-4f);
}

TEST_CASE("Rotate toMatrix rotates a point around Y axis") {
    const Transform transform = Rotate(0.0f, 90.0f, 0.0f);

    const glm::vec4 point(1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 transformed = transform.toMatrix() * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(0.0f, 0.0f, -1.0f), 1e-4f);
}

TEST_CASE("Rotate does not affect translation or scale") {
    const Transform transform = Rotate(45.0f, 30.0f, 60.0f);

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f));
}
