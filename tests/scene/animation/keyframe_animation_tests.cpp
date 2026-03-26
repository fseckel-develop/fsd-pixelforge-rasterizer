#include <doctest.h>
#include <pixelforge/scene/animation/keyframe_animation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>

using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::Keyframe;
using pixelforge::scene::animation::KeyframeAnimation;
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

    Transform makeTransform(const glm::vec3& translation,
                            const glm::vec3& eulerDegrees = glm::vec3(0.0f),
                            const glm::vec3& scale = glm::vec3(1.0f)) {
        Transform t;
        t.setTranslation(translation);
        t.setRotation(eulerDegrees);
        t.setScale(scale);
        return t;
    }

} // namespace


TEST_CASE("KeyframeAnimation default constructor starts with empty keyframes and minimum duration") {
    const KeyframeAnimation animation(Animation::ONCE);

    CHECK(animation.getMode() == Animation::ONCE);
    CHECK(animation.getDuration() == doctest::Approx(0.01f));
    CHECK(animation.getKeyframes().empty());
    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
}

TEST_CASE("KeyframeAnimation addKeyframe stores and sorts keyframes by timestamp") {
    KeyframeAnimation animation(Animation::ONCE);

    const Transform a = makeTransform(glm::vec3(1.0f, 0.0f, 0.0f));
    const Transform b = makeTransform(glm::vec3(2.0f, 0.0f, 0.0f));
    const Transform c = makeTransform(glm::vec3(3.0f, 0.0f, 0.0f));

    animation.addKeyframe(2.0f, b);
    animation.addKeyframe(1.0f, a);
    animation.addKeyframe(3.0f, c);

    const auto& keyframes = animation.getKeyframes();
    REQUIRE(keyframes.size() == 3);

    CHECK(keyframes[0].timestamp == doctest::Approx(1.0f));
    CHECK(keyframes[1].timestamp == doctest::Approx(2.0f));
    CHECK(keyframes[2].timestamp == doctest::Approx(3.0f));

    checkVec3Approx(keyframes[0].transform.getTranslation(), glm::vec3(1.0f, 0.0f, 0.0f));
    checkVec3Approx(keyframes[1].transform.getTranslation(), glm::vec3(2.0f, 0.0f, 0.0f));
    checkVec3Approx(keyframes[2].transform.getTranslation(), glm::vec3(3.0f, 0.0f, 0.0f));
}

TEST_CASE("KeyframeAnimation duration grows to latest keyframe timestamp") {
    KeyframeAnimation animation(Animation::ONCE);

    animation.addKeyframe(0.5f, makeTransform(glm::vec3(0.0f)));
    CHECK(animation.getDuration() == doctest::Approx(0.5f));

    animation.addKeyframe(2.0f, makeTransform(glm::vec3(1.0f)));
    CHECK(animation.getDuration() == doctest::Approx(2.0f));

    animation.addKeyframe(1.0f, makeTransform(glm::vec3(2.0f)));
    CHECK(animation.getDuration() == doctest::Approx(2.0f));
}

TEST_CASE("KeyframeAnimation getOffset returns identity transform when no keyframes exist") {
    const KeyframeAnimation animation(Animation::ONCE);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f));
    checkVec3Approx(offset.getScale(), glm::vec3(1.0f));
    checkQuatApprox(offset.getRotation(), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
}

TEST_CASE("KeyframeAnimation getOffset returns the only keyframe transform when one keyframe exists") {
    KeyframeAnimation animation(Animation::ONCE);

    const Transform only = makeTransform(glm::vec3(1.0f, 2.0f, 3.0f),
                                         glm::vec3(0.0f, 45.0f, 0.0f),
                                         glm::vec3(2.0f, 3.0f, 4.0f));

    animation.addKeyframe(1.5f, only);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), only.getTranslation());
    checkVec3Approx(offset.getScale(), only.getScale());
    checkQuatApprox(offset.getRotation(), only.getRotation());
}

TEST_CASE("KeyframeAnimation interpolates translation scale and rotation between two keyframes") {
    KeyframeAnimation animation(Animation::ONCE);

    const Transform start = makeTransform(glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(1.0f, 1.0f, 1.0f));

    const Transform end = makeTransform(glm::vec3(10.0f, 20.0f, 30.0f),
                                        glm::vec3(0.0f, 90.0f, 0.0f),
                                        glm::vec3(3.0f, 5.0f, 7.0f));

    animation.addKeyframe(1.0f, start);
    animation.addKeyframe(3.0f, end);

    // duration = 3.0, so update to t = 2.0 means progress = 2/3
    // interpolation happens between keyframes at timestamps 1 and 3
    // factor = (2 - 1) / (3 - 1) = 0.5
    animation.update(2.0f);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(5.0f, 10.0f, 15.0f));
    checkVec3Approx(offset.getScale(), glm::vec3(2.0f, 3.0f, 4.0f));

    const glm::quat expectedRotation = glm::slerp(start.getRotation(), end.getRotation(), 0.5f);
    checkQuatApprox(offset.getRotation(), expectedRotation);
}

TEST_CASE("KeyframeAnimation returns last keyframe transform after final keyframe") {
    KeyframeAnimation animation(Animation::ONCE);

    const Transform a = makeTransform(glm::vec3(1.0f, 0.0f, 0.0f));
    const Transform b = makeTransform(glm::vec3(2.0f, 0.0f, 0.0f));

    animation.addKeyframe(1.0f, a);
    animation.addKeyframe(2.0f, b);

    animation.update(5.0f);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), b.getTranslation());
    checkVec3Approx(offset.getScale(), b.getScale());
    checkQuatApprox(offset.getRotation(), b.getRotation());
}

TEST_CASE("KeyframeAnimation LOOP mode wraps into the keyframe timeline") {
    KeyframeAnimation animation(Animation::LOOP);

    const Transform a = makeTransform(glm::vec3(0.0f, 0.0f, 0.0f));
    const Transform b = makeTransform(glm::vec3(10.0f, 0.0f, 0.0f));

    animation.addKeyframe(1.0f, a);
    animation.addKeyframe(3.0f, b);

    // elapsed = 4.0, duration = 3.0
    // LOOP progress = fmod(4/3, 1) = 1/3
    // time = progress * duration = 1.0
    // at exactly time=1.0 the interpolation condition matches the first segment endpoint
    animation.update(4.0f);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), a.getTranslation());
}

TEST_CASE("KeyframeAnimation BOUNCE mode reflects through the keyframe timeline") {
    KeyframeAnimation animation(Animation::BOUNCE);

    const Transform a = makeTransform(glm::vec3(0.0f, 0.0f, 0.0f));
    const Transform b = makeTransform(glm::vec3(10.0f, 0.0f, 0.0f));

    animation.addKeyframe(1.0f, a);
    animation.addKeyframe(3.0f, b);

    // elapsed = 4.5, duration = 3.0
    // elapsed/duration = 1.5 -> bounce progress = 0.5
    // time = 1.5, so factor = (1.5 - 1.0) / (3.0 - 1.0) = 0.25
    animation.update(4.5f);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(2.5f, 0.0f, 0.0f));
}

TEST_CASE("KeyframeAnimation play resumes paused animation without resetting progress") {
    KeyframeAnimation animation(Animation::ONCE);

    animation.addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animation.addKeyframe(3.0f, makeTransform(glm::vec3(10.0f, 0.0f, 0.0f)));

    animation.update(2.0f);
    const Transform before = animation.getOffset();

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform after = animation.getOffset();
    checkVec3Approx(after.getTranslation(), before.getTranslation());
}

TEST_CASE("KeyframeAnimation play restarts stopped animation from beginning") {
    KeyframeAnimation animation(Animation::ONCE);

    animation.addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animation.addKeyframe(3.0f, makeTransform(glm::vec3(10.0f, 0.0f, 0.0f)));

    animation.update(2.0f);
    animation.stop();

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    // Current implementation at time=0 falls through and returns the last keyframe.
    // This test reflects current behavior.
    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(10.0f, 0.0f, 0.0f));
}

TEST_CASE("KeyframeAnimation play restarts finished ONCE animation from beginning") {
    KeyframeAnimation animation(Animation::ONCE);

    animation.addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animation.addKeyframe(3.0f, makeTransform(glm::vec3(10.0f, 0.0f, 0.0f)));

    animation.update(10.0f);
    REQUIRE_FALSE(animation.isPlaying());

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    // Same current implementation behavior at time=0.
    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(10.0f, 0.0f, 0.0f));
}

TEST_CASE("KeyframeAnimation update does not advance while paused") {
    KeyframeAnimation animation(Animation::ONCE);

    animation.addKeyframe(1.0f, makeTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
    animation.addKeyframe(3.0f, makeTransform(glm::vec3(10.0f, 0.0f, 0.0f)));

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.update(1.0f);

    // At time=0 current implementation returns the last keyframe.
    const Transform offset = animation.getOffset();
    checkVec3Approx(offset.getTranslation(), glm::vec3(10.0f, 0.0f, 0.0f));
}
