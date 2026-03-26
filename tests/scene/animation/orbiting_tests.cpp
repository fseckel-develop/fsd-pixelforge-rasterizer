#include <doctest.h>
#include <pixelforge/scene/animation/orbiting.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::Orbiting;
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


TEST_CASE("Orbiting default constructor sets zero radius Y axis and full turn") {
    const Orbiting animation(Animation::ONCE);

    CHECK(animation.getRadius() == doctest::Approx(0.0f));
    checkVec3Approx(animation.getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(animation.getTotalAngle() == doctest::Approx(360.0f));
    CHECK(animation.getDuration() == doctest::Approx(1.0f));
    CHECK(animation.getMode() == Animation::ONCE);
}

TEST_CASE("Orbiting custom constructor stores radius axis angle and duration") {
    const Orbiting animation(Animation::LOOP, 2.0f, 5.0f, glm::vec3(0.0f, 2.0f, 0.0f), 180.0f);

    CHECK(animation.getRadius() == doctest::Approx(5.0f));
    checkVec3Approx(animation.getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(animation.getTotalAngle() == doctest::Approx(180.0f));
    CHECK(animation.getDuration() == doctest::Approx(2.0f));
    CHECK(animation.getMode() == Animation::LOOP);
}

TEST_CASE("Orbiting setRadius updates radius") {
    Orbiting animation(Animation::ONCE);

    animation.setRadius(7.5f);

    CHECK(animation.getRadius() == doctest::Approx(7.5f));
}

TEST_CASE("Orbiting offset has zero translation when radius is zero") {
    const Orbiting animation(Animation::ONCE, 2.0f, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f));
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Orbiting offset starts on positive Z for Y axis orbit") {
    const Orbiting animation(Animation::ONCE, 2.0f, 3.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, -3.0f));
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("Orbiting offset at half progress rotates to opposite side of orbit") {
    Orbiting animation(Animation::ONCE, 2.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(1.0f); // 50% -> 90 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(-2.0f, 0.0f, 0.0f), 1e-4f);

    const glm::quat expectedRotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    checkQuatApprox(offset.getRotation(), expectedRotation, 1e-4f);
}

TEST_CASE("Orbiting offset at full progress in ONCE mode reaches final orbit position") {
    Orbiting animation(Animation::ONCE, 2.0f, 4.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(2.0f); // 100% -> 180 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, 4.0f), 1e-4f);

    const glm::quat expectedRotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    checkQuatApprox(offset.getRotation(), expectedRotation, 1e-4f);
}

TEST_CASE("Orbiting LOOP mode wraps progress") {
    Orbiting animation(Animation::LOOP, 2.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(2.5f); // progress = 0.25 -> 45 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(
        offset.getTranslation(),
        glm::vec3(-std::sqrt(2.0f), 0.0f, -std::sqrt(2.0f)),
        1e-4f
    );

    const glm::quat expectedRotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    checkQuatApprox(offset.getRotation(), expectedRotation, 1e-4f);
}

TEST_CASE("Orbiting BOUNCE mode reflects progress") {
    Orbiting animation(Animation::BOUNCE, 2.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(3.0f); // elapsed/duration = 1.5 -> bounce progress = 0.5 -> 90 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(-2.0f, 0.0f, 0.0f), 1e-4f);

    const glm::quat expectedRotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    checkQuatApprox(offset.getRotation(), expectedRotation, 1e-4f);
}

TEST_CASE("Orbiting chooses alternate radiant basis when axis is near X") {
    const Orbiting animation(Animation::ONCE, 2.0f, 3.0f, glm::vec3(1.0f, 0.0f, 0.0f), 180.0f);

    const Transform offset = animation.getOffset();

    // Since axis is X, implementation switches radiant helper to Y,
    // so cross(X, Y) = +Z and starting orbital position is again +Z * radius.
    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, 3.0f), 1e-4f);
}

TEST_CASE("Orbiting play resumes paused animation without resetting progress") {
    Orbiting animation(Animation::ONCE, 2.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(0.5f); // 25%
    animation.pause();
    REQUIRE(animation.isPaused());

    const Transform before = animation.getOffset();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform after = animation.getOffset();
    checkVec3Approx(after.getTranslation(), before.getTranslation(), 1e-4f);
    checkQuatApprox(after.getRotation(), before.getRotation(), 1e-4f);
}

TEST_CASE("Orbiting play restarts stopped animation from beginning") {
    Orbiting animation(Animation::ONCE, 2.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(0.5f); // 25%
    animation.stop();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, -2.0f), 1e-4f);
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("Orbiting play restarts finished ONCE animation from beginning") {
    Orbiting animation(Animation::ONCE, 1.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.update(2.0f);
    REQUIRE_FALSE(animation.isPlaying());

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, -2.0f), 1e-4f);
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("Orbiting update does not advance while paused") {
    Orbiting animation(Animation::ONCE, 2.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.update(1.0f);

    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, -2.0f), 1e-4f);
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 1e-4f);
}
