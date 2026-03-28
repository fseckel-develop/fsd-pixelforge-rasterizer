#include <doctest.h>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>

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

    void checkMat4Approx(const glm::mat4& actual, const glm::mat4& expected, float eps = EPS) {
        for (int c = 0; c < 4; ++c) {
            for (int r = 0; r < 4; ++r) {
                CHECK(actual[c][r] == doctest::Approx(expected[c][r]).epsilon(eps));
            }
        }
    }

    glm::vec3 composeTranslation(const Transform& parent, const Transform& child) {
        return parent.getTranslation() + parent.getRotation() * (parent.getScale() * child.getTranslation());
    }

} // namespace


TEST_CASE("Transform default constructor creates identity transform") {
    const Transform transform;

    checkVec3Approx(transform.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
    checkQuatApprox(transform.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

    checkMat4Approx(transform.toMatrix(), glm::mat4(1.0f));
}

TEST_CASE("Transform setters update translation rotation and scale") {
    Transform transform;

    transform.setTranslation(1.0f, 2.0f, 3.0f);
    transform.setRotation(0.0f, 90.0f, 0.0f);
    transform.setScale(2.0f, 3.0f, 4.0f);

    checkVec3Approx(transform.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));

    const glm::quat expectedRotation = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
    checkQuatApprox(transform.getRotation(), expectedRotation, 1e-4f);
}

TEST_CASE("Transform uniform scale setter sets all scale components equally") {
    Transform transform;

    transform.setScale(2.5f);

    checkVec3Approx(transform.getScale(), glm::vec3(2.5f, 2.5f, 2.5f));
}

TEST_CASE("Transform constructor from translation euler angles and scale stores expected values") {
    const Transform transform(
        glm::vec3(1.0f, 2.0f, 3.0f),
        glm::vec3(0.0f, 45.0f, 90.0f),
        glm::vec3(2.0f, 3.0f, 4.0f)
    );

    checkVec3Approx(transform.getTranslation(), glm::vec3(1.0f, 2.0f, 3.0f));
    checkVec3Approx(transform.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));

    // Adjusted to match the corrected degree-based rotation semantics.
    const glm::quat expectedRotation = glm::quat(glm::radians(glm::vec3(0.0f, 45.0f, 90.0f)));
    checkQuatApprox(transform.getRotation(), expectedRotation, 1e-4f);
}

TEST_CASE("Transform toMatrix matches GLM translation rotation and scale composition") {
    Transform transform;
    transform.setTranslation(1.0f, 2.0f, 3.0f);
    transform.setRotation(0.0f, 90.0f, 0.0f);
    transform.setScale(2.0f, 3.0f, 4.0f);

    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
    expected *= glm::mat4_cast(glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f))));
    expected = glm::scale(expected, glm::vec3(2.0f, 3.0f, 4.0f));

    checkMat4Approx(transform.toMatrix(), expected, 1e-4f);
}

TEST_CASE("Transform composeFromMatrix recovers translation and scale") {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, glm::vec3(4.0f, 5.0f, 6.0f));
    matrix *= glm::mat4_cast(glm::quat(glm::radians(glm::vec3(10.0f, 20.0f, 30.0f))));
    matrix = glm::scale(matrix, glm::vec3(2.0f, 3.0f, 4.0f));

    Transform transform;
    transform.composeFromMatrix(matrix);

    checkVec3Approx(transform.getTranslation(), glm::vec3(4.0f, 5.0f, 6.0f), 1e-4f);
    checkVec3Approx(transform.getScale(), glm::vec3(2.0f, 3.0f, 4.0f), 1e-4f);
}

TEST_CASE("Transform matrix constructor composes from matrix") {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, glm::vec3(-1.0f, 2.0f, -3.0f));
    matrix *= glm::mat4_cast(glm::quat(glm::radians(glm::vec3(15.0f, 25.0f, 35.0f))));
    matrix = glm::scale(matrix, glm::vec3(1.5f, 2.5f, 3.5f));

    const Transform transform(matrix);

    checkVec3Approx(transform.getTranslation(), glm::vec3(-1.0f, 2.0f, -3.0f), 1e-4f);
    checkVec3Approx(transform.getScale(), glm::vec3(1.5f, 2.5f, 3.5f), 1e-4f);
}

TEST_CASE("Transform operator composes hierarchical transforms correctly") {
    Transform a;
    a.setTranslation(1.0f, 2.0f, 3.0f);
    a.setRotation(0.0f, 90.0f, 0.0f);
    a.setScale(2.0f, 3.0f, 4.0f);

    Transform b;
    b.setTranslation(4.0f, 5.0f, 6.0f);
    b.setRotation(0.0f, 0.0f, 90.0f);
    b.setScale(5.0f, 6.0f, 7.0f);

    const Transform c = a * b;

    const glm::vec3 expectedTranslation = composeTranslation(a, b);
    checkVec3Approx(c.getTranslation(), expectedTranslation);
    checkVec3Approx(c.getScale(), glm::vec3(10.0f, 18.0f, 28.0f));

    const glm::quat expectedRotation = a.getRotation() * b.getRotation();
    checkQuatApprox(c.getRotation(), expectedRotation, 1e-4f);
}

TEST_CASE("Transform toMatrix transforms a point as expected for translation only") {
    Transform transform;
    transform.setTranslation(3.0f, -2.0f, 5.0f);

    constexpr glm::vec4 point(1.0f, 2.0f, 3.0f, 1.0f);
    const glm::vec4 transformed = transform.toMatrix() * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(4.0f, 0.0f, 8.0f));
}

TEST_CASE("Transform toMatrix transforms a point as expected for uniform scale only") {
    Transform transform;
    transform.setScale(2.0f);

    constexpr glm::vec4 point(1.0f, 2.0f, 3.0f, 1.0f);
    const glm::vec4 transformed = transform.toMatrix() * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(2.0f, 4.0f, 6.0f));
}

TEST_CASE("Transform toMatrix transforms a point as expected for rotation around Z") {
    Transform transform;
    transform.setRotation(0.0f, 0.0f, 90.0f);

    constexpr glm::vec4 point(1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 transformed = transform.toMatrix() * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(0.0f, 1.0f, 0.0f), 1e-4f);
}

TEST_CASE("Transform composition rotates child translation into parent space") {
    Transform parent;
    parent.setRotation(glm::vec3(0.0f, 90.0f, 0.0f));

    Transform child;
    child.setTranslation(glm::vec3(3.0f, 0.0f, 0.0f));

    const Transform combined = parent * child;
    const glm::vec3 t = combined.getTranslation();

    CHECK(t.x == doctest::Approx(0.0f).epsilon(1e-4f));
    CHECK(t.y == doctest::Approx(0.0f).epsilon(1e-4f));
    CHECK(t.z == doctest::Approx(-3.0f).epsilon(1e-4f));
}
