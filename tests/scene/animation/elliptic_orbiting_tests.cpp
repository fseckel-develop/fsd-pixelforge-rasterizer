#include <doctest.h>
#include <pixelforge/scene/animation/elliptic_orbiting.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::EllipticOrbiting;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, const float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

    void checkQuatApprox(const glm::quat& actual, const glm::quat& expected, const float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
        CHECK(actual.w == doctest::Approx(expected.w).epsilon(eps));
    }

} // namespace


TEST_CASE("EllipticOrbiting default constructor sets default axes Y axis and full turn") {
    const EllipticOrbiting animation(Animation::ONCE);

    CHECK(animation.getSemiMajorAxis() == doctest::Approx(1.0f));
    CHECK(animation.getSemiMinorAxis() == doctest::Approx(0.5f));
    checkVec3Approx(animation.getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(animation.getTotalAngle() == doctest::Approx(360.0f));
    CHECK(animation.getDuration() == doctest::Approx(1.0f));
    CHECK(animation.getMode() == Animation::ONCE);
    CHECK_FALSE(animation.getUseFocusOrigin());
}

TEST_CASE("EllipticOrbiting custom constructor stores axes axis angle duration and focus mode") {
    const EllipticOrbiting animation(
        Animation::LOOP, 2.0f, 5.0f, 3.0f, glm::vec3(0.0f, 2.0f, 0.0f), 180.0f, true
    );

    CHECK(animation.getSemiMajorAxis() == doctest::Approx(5.0f));
    CHECK(animation.getSemiMinorAxis() == doctest::Approx(3.0f));
    checkVec3Approx(animation.getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(animation.getTotalAngle() == doctest::Approx(180.0f));
    CHECK(animation.getDuration() == doctest::Approx(2.0f));
    CHECK(animation.getMode() == Animation::LOOP);
    CHECK(animation.getUseFocusOrigin());
}

TEST_CASE("EllipticOrbiting setSemiMajorAxis updates semi-major axis") {
    EllipticOrbiting animation(Animation::ONCE);

    animation.setSemiMajorAxis(7.5f);

    CHECK(animation.getSemiMajorAxis() == doctest::Approx(7.5f));
}

TEST_CASE("EllipticOrbiting setSemiMinorAxis updates semi-minor axis") {
    EllipticOrbiting animation(Animation::ONCE);

    animation.setSemiMinorAxis(4.5f);

    CHECK(animation.getSemiMinorAxis() == doctest::Approx(4.5f));
}

TEST_CASE("EllipticOrbiting setAxes updates both axes") {
    EllipticOrbiting animation(Animation::ONCE);

    animation.setAxes(8.0f, 6.0f);

    CHECK(animation.getSemiMajorAxis() == doctest::Approx(8.0f));
    CHECK(animation.getSemiMinorAxis() == doctest::Approx(6.0f));
}

TEST_CASE("EllipticOrbiting setUseFocusOrigin updates focus mode") {
    EllipticOrbiting animation(Animation::ONCE);

    animation.setUseFocusOrigin(true);
    CHECK(animation.getUseFocusOrigin());

    animation.setUseFocusOrigin(false);
    CHECK_FALSE(animation.getUseFocusOrigin());
}

TEST_CASE("EllipticOrbiting offset starts on positive X when centered") {
    const EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(4.0f, 0.0f, 0.0f), 1e-4f);
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting offset starts at focus-shifted position when focus origin is enabled") {
    const EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, true
    );

    const Transform offset = animation.getOffset();

    const float focalDistance = std::sqrt(4.0f * 4.0f - 2.0f * 2.0f);
    checkVec3Approx(offset.getTranslation(), glm::vec3(4.0f - focalDistance, 0.0f, 0.0f), 1e-4f);
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting offset at half progress reaches top of ellipse for Y axis orbit") {
    EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.update(1.0f); // 50% -> 90 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, 2.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting offset at full progress in ONCE mode reaches opposite side of ellipse") {
    EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.update(2.0f); // 100% -> 180 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(-4.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting full progress with focus origin reaches focus-shifted opposite side") {
    EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, true
    );

    animation.update(2.0f);

    const Transform offset = animation.getOffset();

    const float focalDistance = std::sqrt(4.0f * 4.0f - 2.0f * 2.0f);
    checkVec3Approx(offset.getTranslation(), glm::vec3(-4.0f - focalDistance, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting LOOP mode wraps progress") {
    EllipticOrbiting animation(
        Animation::LOOP, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.update(2.5f); // progress = 0.25 -> 45 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(
        offset.getTranslation(),
        glm::vec3(4.0f * std::sqrt(2.0f) / 2.0f, 0.0f, 2.0f * std::sqrt(2.0f) / 2.0f),
        1e-4f
    );
}

TEST_CASE("EllipticOrbiting BOUNCE mode reflects progress") {
    EllipticOrbiting animation(
        Animation::BOUNCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.update(3.0f); // elapsed/duration = 1.5 -> bounce progress = 0.5 -> 90 degrees

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, 2.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting rotates orbit plane when axis is X") {
    const EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 180.0f, false
    );

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, -4.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting rotates orbit plane when axis is Z") {
    EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 0.0f, 1.0f), 180.0f, false
    );

    animation.update(1.0f); // 90 degrees in local orbit plane: (0, 0, 2)

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, -2.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting play resumes paused animation without resetting progress") {
    EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.update(0.5f); // 25%
    animation.pause();
    REQUIRE(animation.isPaused());

    const Transform before = animation.getOffset();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform after = animation.getOffset();
    checkVec3Approx(after.getTranslation(), before.getTranslation(), 1e-4f);
}

TEST_CASE("EllipticOrbiting play restarts stopped animation from beginning") {
    EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.update(0.5f); // 25%
    animation.stop();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(4.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting play restarts finished ONCE animation from beginning") {
    EllipticOrbiting animation(
        Animation::ONCE, 1.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.update(2.0f);
    REQUIRE_FALSE(animation.isPlaying());

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(4.0f, 0.0f, 0.0f), 1e-4f);
}

TEST_CASE("EllipticOrbiting update does not advance while paused") {
    EllipticOrbiting animation(
        Animation::ONCE, 2.0f, 4.0f, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, false
    );

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.update(1.0f);

    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(4.0f, 0.0f, 0.0f), 1e-4f);
}
