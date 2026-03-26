#include <doctest.h>
#include <pixelforge/scene/animation/translation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::Translation;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected, float eps = EPS) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(eps));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(eps));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(eps));
    }

} // namespace


TEST_CASE("Translation default constructor sets forward direction and unit distance") {
    const Translation animation(Animation::ONCE);

    checkVec3Approx(animation.getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
    CHECK(animation.getTotalDistance() == doctest::Approx(1.0f));
    CHECK(animation.getDuration() == doctest::Approx(1.0f));
    CHECK(animation.getMode() == Animation::ONCE);
}

TEST_CASE("Translation custom constructor stores normalized direction and distance") {
    const Translation animation(Animation::LOOP, 2.0f, glm::vec3(0.0f, 0.0f, 3.0f), 5.0f);

    checkVec3Approx(animation.getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
    CHECK(animation.getTotalDistance() == doctest::Approx(5.0f));
    CHECK(animation.getDuration() == doctest::Approx(2.0f));
    CHECK(animation.getMode() == Animation::LOOP);
}

TEST_CASE("Translation setDirection updates normalized direction") {
    Translation animation(Animation::ONCE);

    animation.setDirection(glm::vec3(4.0f, 0.0f, 0.0f));

    checkVec3Approx(animation.getDirection(), glm::vec3(1.0f, 0.0f, 0.0f));
}

TEST_CASE("Translation setTotalDistance updates distance") {
    Translation animation(Animation::ONCE);

    animation.setTotalDistance(7.5f);

    CHECK(animation.getTotalDistance() == doctest::Approx(7.5f));
}

TEST_CASE("Translation offset is zero at zero progress") {
    const Translation animation(Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f));
    checkVec3Approx(offset.getScale(), glm::vec3(1.0f));
}

TEST_CASE("Translation offset matches half progress movement") {
    Translation animation(Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(5.0f, 0.0f, 0.0f));
}

TEST_CASE("Translation offset matches full progress in ONCE mode") {
    Translation animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 6.0f);

    animation.update(2.0f); // 100%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 6.0f, 0.0f));
}

TEST_CASE("Translation LOOP mode wraps progress in getOffset") {
    Translation animation(Animation::LOOP, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 8.0f);

    animation.update(2.5f); // progress = 0.25

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(2.0f, 0.0f, 0.0f));
}

TEST_CASE("Translation BOUNCE mode reflects progress in getOffset") {
    Translation animation(Animation::BOUNCE, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), 8.0f);

    animation.update(3.0f); // elapsed/duration = 1.5 -> bounce progress = 0.5

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 4.0f, 0.0f));
}

TEST_CASE("Translation offset uses normalized direction") {
    Translation animation(Animation::ONCE, 2.0f, glm::vec3(0.0f, 0.0f, 10.0f), 6.0f);

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, 3.0f));
}

TEST_CASE("Translation play resumes paused animation without resetting progress") {
    Translation animation(Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);

    animation.update(0.5f); // 25%
    animation.pause();
    REQUIRE(animation.isPaused());

    const Transform before = animation.getOffset();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform after = animation.getOffset();
    checkVec3Approx(after.getTranslation(), before.getTranslation());
}

TEST_CASE("Translation play restarts stopped animation from beginning") {
    Translation animation(Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);

    animation.update(0.5f); // 25%
    CHECK(animation.getOffset().getTranslation().x == doctest::Approx(2.5f));

    animation.stop();
    CHECK(animation.getOffset().getTranslation().x == doctest::Approx(0.0f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(0.0f));
}

TEST_CASE("Translation play restarts finished ONCE animation from beginning") {
    Translation animation(Animation::ONCE, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), 4.0f);

    animation.update(2.0f);
    REQUIRE_FALSE(animation.isPlaying());
    CHECK(animation.getOffset().getTranslation().z == doctest::Approx(4.0f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(0.0f));
}

TEST_CASE("Translation update does not advance while paused") {
    Translation animation(Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.update(1.0f);

    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(0.0f));
}

TEST_CASE("Translation supports negative total distance") {
    Translation animation(Animation::ONCE, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f), -6.0f);

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(-3.0f, 0.0f, 0.0f));
}
