#include <doctest.h>
#include <pixelforge/geometry/curves/bezier_curve.hpp>
#include <pixelforge/scene/animation/curve_animation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <vector>

using pixelforge::geometry::BezierCurve;
using pixelforge::scene::animation::Animation;
using pixelforge::scene::animation::CurveAnimation;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-4f;

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

    std::shared_ptr<BezierCurve> makeSimpleLineCurve() {
        return std::make_shared<BezierCurve>(
            std::vector<glm::vec3>{
                {0.0f, 0.0f, 0.0f},
                {2.0f, 0.0f, 0.0f},
                {4.0f, 0.0f, 0.0f},
                {6.0f, 0.0f, 0.0f}
            }
        );
    }

} // namespace


TEST_CASE("CurveAnimation constructor stores curve duration and mode") {
    const auto curve = makeSimpleLineCurve();

    const CurveAnimation animation(curve, 2.0f, Animation::LOOP);

    CHECK(animation.getCurve() == curve);
    CHECK(animation.getDuration() == doctest::Approx(2.0f));
    CHECK(animation.getMode() == Animation::LOOP);
}

TEST_CASE("CurveAnimation setCurve updates curve") {
    const auto curveA = makeSimpleLineCurve();
    const auto curveB = std::make_shared<BezierCurve>(
        std::vector<glm::vec3>{
            {1.0f, 0.0f, 0.0f},
            {1.0f, 2.0f, 0.0f},
            {1.0f, 4.0f, 0.0f},
            {1.0f, 6.0f, 0.0f}
        }
    );

    CurveAnimation animation(curveA, 2.0f, Animation::ONCE);
    REQUIRE(animation.getCurve() == curveA);

    animation.setCurve(curveB);

    CHECK(animation.getCurve() == curveB);
}

TEST_CASE("CurveAnimation offset translation matches curve start at zero progress") {
    const auto curve = makeSimpleLineCurve();
    const CurveAnimation animation(curve, 2.0f, Animation::ONCE);

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
}

TEST_CASE("CurveAnimation offset translation matches curve midpoint at half progress") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::ONCE);

    animation.update(1.0f); // 50%

    const Transform offset = animation.getOffset();

    // For this straight cubic Bezier, midpoint is x = 3
    checkVec3Approx(offset.getTranslation(), glm::vec3(3.0f, 0.0f, 0.0f));
}

TEST_CASE("CurveAnimation offset translation matches curve end at full progress in ONCE mode") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::ONCE);

    animation.update(2.0f); // 100%

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(6.0f, 0.0f, 0.0f));
}

TEST_CASE("CurveAnimation LOOP mode wraps progress along the curve") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::LOOP);

    animation.update(2.5f); // progress = 0.25

    const Transform offset = animation.getOffset();

    // 25% along line from x=0 to x=6
    checkVec3Approx(offset.getTranslation(), glm::vec3(1.5f, 0.0f, 0.0f));
}

TEST_CASE("CurveAnimation BOUNCE mode reflects progress along the curve") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::BOUNCE);

    animation.update(3.0f); // elapsed/duration = 1.5 -> bounce progress = 0.5

    const Transform offset = animation.getOffset();

    checkVec3Approx(offset.getTranslation(), glm::vec3(3.0f, 0.0f, 0.0f));
}

TEST_CASE("CurveAnimation offset rotation matches curve rotation minimizing frame on straight line") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::ONCE);

    animation.update(1.0f); // midpoint

    const Transform offset = animation.getOffset();

    // For this straight line along +X, the initial/default rotation minimizing
    // frame should remain stable and match the curve's frame at the same t.
    const glm::quat expected = glm::quat_cast(curve->computeRotMinFrame(0.5f));

    checkQuatApprox(offset.getRotation(), expected);
}

TEST_CASE("CurveAnimation play resumes paused animation without resetting progress") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::ONCE);

    animation.update(0.5f); // 25%
    const Transform before = animation.getOffset();

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());

    const Transform after = animation.getOffset();
    checkVec3Approx(after.getTranslation(), before.getTranslation());
}

TEST_CASE("CurveAnimation play restarts stopped animation from beginning") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::ONCE);

    animation.update(0.5f); // 25%
    CHECK(animation.getOffset().getTranslation().x == doctest::Approx(1.5f).epsilon(EPS));

    animation.stop();
    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
}

TEST_CASE("CurveAnimation play restarts finished ONCE animation from beginning") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 1.0f, Animation::ONCE);

    animation.update(2.0f);
    REQUIRE_FALSE(animation.isPlaying());
    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(6.0f, 0.0f, 0.0f));

    animation.play();

    CHECK(animation.isPlaying());
    CHECK_FALSE(animation.isPaused());
    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
}

TEST_CASE("CurveAnimation update does not advance while paused") {
    const auto curve = makeSimpleLineCurve();
    CurveAnimation animation(curve, 2.0f, Animation::ONCE);

    animation.pause();
    REQUIRE(animation.isPaused());

    animation.update(1.0f);

    checkVec3Approx(animation.getOffset().getTranslation(), glm::vec3(0.0f, 0.0f, 0.0f));
}
