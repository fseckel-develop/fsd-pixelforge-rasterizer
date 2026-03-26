#include <doctest.h>
#include <pixelforge/scene/animation/scaling.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::Scaling;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

} // namespace


TEST_CASE("Scaling default constructor sets unit target scale") {
    const Scaling animation(Animation::ONCE);

    checkVec3Approx(animation.getTargetScale(), glm::vec3(1.0f, 1.0f, 1.0f));
    CHECK(animation.getDuration() == doctest::Approx(1.0f));
    CHECK(animation.getMode() == Animation::ONCE);
}

TEST_CASE("Scaling custom constructor stores target scale and duration") {
    const Scaling animation(Animation::LOOP, 2.0f, glm::vec3(2.0f, 3.0f, 4.0f));

    checkVec3Approx(animation.getTargetScale(), glm::vec3(2.0f, 3.0f, 4.0f));
    CHECK(animation.getDuration() == doctest::Approx(2.0f));
    CHECK(animation.getMode() == Animation::LOOP);
}

TEST_CASE("Scaling setTargetScale scalar updates uniform target scale") {
    Scaling animation(Animation::ONCE);

    animation.setTargetScale(2.5f);

    checkVec3Approx(animation.getTargetScale(), glm::vec3(2.5f, 2.5f, 2.5f));
}

TEST_CASE("Scaling setTargetScale vector updates target scale") {
    Scaling animation(Animation::ONCE);

    animation.setTargetScale(glm::vec3(2.0f, 3.0f, 4.0f));

    checkVec3Approx(animation.getTargetScale(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("Scaling offset is identity scale at zero progress") {
    const Scaling animation(Animation::ONCE, 2.0f, glm::vec3(2.0f, 3.0f, 4.0f));

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
}

TEST_CASE("Scaling offset interpolates halfway between unit and target scale") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(3.0f, 5.0f, 7.0f));

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("Scaling offset matches target scale at full progress in ONCE mode") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(2.0f, 4.0f, 6.0f));

    animation.update(2.0f); // 100%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getScale(), glm::vec3(2.0f, 4.0f, 6.0f));
}

TEST_CASE("Scaling LOOP mode wraps progress in getOffset") {
    Scaling animation(Animation::LOOP, 2.0f, glm::vec3(3.0f, 3.0f, 3.0f));

    animation.update(2.5f); // progress = 0.25

    const Transform offset = animation.getOffset();

    // mix(1, 3, 0.25) = 1.5
    checkVec3Approx(offset.getScale(), glm::vec3(1.5f, 1.5f, 1.5f));
}

TEST_CASE("Scaling BOUNCE mode reflects progress in getOffset") {
    Scaling animation(Animation::BOUNCE, 2.0f, glm::vec3(5.0f, 5.0f, 5.0f));

    animation.update(3.0f); // elapsed/duration = 1.5 -> bounce progress = 0.5

    const Transform offset = animation.getOffset();

    // mix(1, 5, 0.5) = 3
    checkVec3Approx(offset.getScale(), glm::vec3(3.0f, 3.0f, 3.0f));
}

TEST_CASE("Scaling supports non-uniform target scale") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(2.0f, 3.0f, 4.0f));

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getScale(), glm::vec3(1.5f, 2.0f, 2.5f));
}

TEST_CASE("Scaling supports shrinking below unit scale") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 0.5f, 1.0f));

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getScale(), glm::vec3(0.5f, 0.75f, 1.0f));
}

TEST_CASE("Scaling supports negative target scale") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(-1.0f, 1.0f, 3.0f));

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getScale(), glm::vec3(0.0f, 1.0f, 2.0f));
}

TEST_CASE("Scaling play resumes paused animation without resetting progress") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(3.0f, 3.0f, 3.0f));

    animation.update(0.5f); // 25%
    animation.pause();
    REQUIRE(animation.isPaused());

    const Transform before = animation.getOffset();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform after = animation.getOffset();
    checkVec3Approx(after.getScale(), before.getScale());
}

TEST_CASE("Scaling play restarts stopped animation from beginning") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(5.0f, 5.0f, 5.0f));

    animation.update(0.5f); // 25%
    checkVec3Approx(animation.getOffset().getScale(), glm::vec3(2.0f, 2.0f, 2.0f));

    animation.stop();
    checkVec3Approx(animation.getOffset().getScale(), glm::vec3(1.0f, 1.0f, 1.0f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    checkVec3Approx(animation.getOffset().getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
}

TEST_CASE("Scaling play restarts finished ONCE animation from beginning") {
    Scaling animation(Animation::ONCE, 1.0f, glm::vec3(4.0f, 4.0f, 4.0f));

    animation.update(2.0f);
    REQUIRE_FALSE(animation.isPlaying());
    checkVec3Approx(animation.getOffset().getScale(), glm::vec3(4.0f, 4.0f, 4.0f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    checkVec3Approx(animation.getOffset().getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
}

TEST_CASE("Scaling update does not advance while paused") {
    Scaling animation(Animation::ONCE, 2.0f, glm::vec3(3.0f, 3.0f, 3.0f));

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.update(1.0f);

    checkVec3Approx(animation.getOffset().getScale(), glm::vec3(1.0f, 1.0f, 1.0f));
}
