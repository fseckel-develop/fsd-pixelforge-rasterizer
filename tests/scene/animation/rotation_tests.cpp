#include <doctest.h>
#include <pixelforge/scene/animation/rotation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::Rotation;
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


TEST_CASE("Rotation default constructor sets Y axis and full turn in radians") {
    const Rotation animation(Animation::ONCE);

    checkVec3Approx(animation.getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(animation.getTotalAngle() == doctest::Approx(360.0f).epsilon(EPS));
    CHECK(animation.getDuration() == doctest::Approx(1.0f));
    CHECK(animation.getMode() == Animation::ONCE);
}

TEST_CASE("Rotation custom constructor stores normalized axis and angle in radians") {
    const Rotation animation(Animation::LOOP, 2.0f, glm::vec3(0.0f, 2.0f, 0.0f), 180.0f);

    checkVec3Approx(animation.getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(animation.getTotalAngle() == doctest::Approx(180.0f).epsilon(EPS));
    CHECK(animation.getDuration() == doctest::Approx(2.0f));
    CHECK(animation.getMode() == Animation::LOOP);
}

TEST_CASE("Rotation setRotationAxis updates normalized axis") {
    Rotation animation(Animation::ONCE);

    animation.setRotationAxis(glm::vec3(3.0f, 0.0f, 0.0f));

    checkVec3Approx(animation.getRotationAxis(), glm::vec3(1.0f, 0.0f, 0.0f));
}

TEST_CASE("Rotation setTotalAngle stores radians") {
    Rotation animation(Animation::ONCE);

    animation.setTotalAngle(90.0f);

    CHECK(animation.getTotalAngle() == doctest::Approx(90.0f).epsilon(EPS));
}

TEST_CASE("Rotation offset is identity at zero progress") {
    Rotation animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    const Transform offset = animation.getOffset();

    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f));
    checkVec3Approx(offset.getScale(), glm::vec3(1.0f));
}

TEST_CASE("Rotation offset matches half progress rotation around Y axis") {
    Rotation animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(1.0f); // 50% progress -> 90 degrees

    const Transform offset = animation.getOffset();
    const glm::quat expected = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    checkQuatApprox(offset.getRotation(), expected, 1e-4f);
}

TEST_CASE("Rotation offset matches full progress rotation in ONCE mode") {
    Rotation animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 0.0f, 1.0f), 180.0f);

    animation.update(2.0f); // 100% progress

    const Transform offset = animation.getOffset();
    const glm::quat expected = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    checkQuatApprox(offset.getRotation(), expected, 1e-4f);
}

TEST_CASE("Rotation LOOP mode wraps progress in getOffset") {
    Rotation animation(Animation::LOOP, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(2.5f); // progress = 0.25 -> 45 degrees

    const Transform offset = animation.getOffset();
    const glm::quat expected = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    checkQuatApprox(offset.getRotation(), expected, 1e-4f);
}

TEST_CASE("Rotation BOUNCE mode reflects progress in getOffset") {
    Rotation animation(Animation::BOUNCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 180.0f);

    animation.update(3.0f); // elapsed/duration = 1.5 -> bounce progress = 0.5 -> 90 degrees

    const Transform offset = animation.getOffset();
    const glm::quat expected = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    checkQuatApprox(offset.getRotation(), expected, 1e-4f);
}

TEST_CASE("Rotation offset rotates a point around Z axis correctly") {
    Rotation animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 0.0f, 1.0f), 180.0f);

    animation.update(1.0f); // 50% -> 90 degrees

    const glm::mat4 matrix = animation.getOffset().toMatrix();
    constexpr glm::vec4 point(1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 transformed = matrix * point;

    checkVec3Approx(glm::vec3(transformed), glm::vec3(0.0f, 1.0f, 0.0f), 1e-4f);
}

TEST_CASE("Rotation play resumes paused animation without resetting progress") {
    Rotation animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(0.5f); // 25%
    animation.pause();
    REQUIRE(animation.isPaused());

    const Transform before = animation.getOffset();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform after = animation.getOffset();
    checkQuatApprox(after.getRotation(), before.getRotation(), 1e-4f);
}

TEST_CASE("Rotation play restarts finished ONCE animation from beginning") {
    Rotation animation(Animation::ONCE, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(2.0f);
    REQUIRE_FALSE(animation.isPlaying());

    animation.play();

    const Transform offset = animation.getOffset();
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 1e-4f);
}
