#include <doctest.h>
#include <pixelforge/builders/animation.hpp>
#include <pixelforge/builders/specification/animation_spec.hpp>
#include <builders/compiler/animation_compiler.hpp>
#include <pixelforge/geometry/curves/bezier_curve.hpp>
#include <pixelforge/scene/animation/curve_animation.hpp>
#include <pixelforge/scene/animation/keyframe_animation.hpp>
#include <pixelforge/scene/animation/orbiting.hpp>
#include <pixelforge/scene/animation/rotation.hpp>
#include <pixelforge/scene/animation/scaling.hpp>
#include <pixelforge/scene/animation/translation.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <vector>

using pixelforge::builders::CurveAnimation;
using pixelforge::builders::KeyframeAnimation;
using pixelforge::builders::Orbiting;
using pixelforge::builders::Rotation;
using pixelforge::builders::Scaling;
using pixelforge::builders::Translation;
using pixelforge::builders::specification::AnimationSpec;
using pixelforge::builders::specification::CurveAnimationSpec;
using pixelforge::builders::specification::KeyframeAnimationSpec;
using pixelforge::builders::specification::OrbitingAnimationSpec;
using pixelforge::builders::specification::RotationAnimationSpec;
using pixelforge::builders::specification::ScalingAnimationSpec;
using pixelforge::builders::specification::TranslationAnimationSpec;
using pixelforge::geometry::BezierCurve;
using pixelforge::scene::animation::Animation;
using pixelforge::scene::builders::compiler::buildAnimation;
using pixelforge::scene::transform::Transform;


namespace {

    constexpr float EPS = 1e-5f;

    void checkVec3Approx(const glm::vec3& actual, const glm::vec3& expected) {
        CHECK(actual.x == doctest::Approx(expected.x).epsilon(EPS));
        CHECK(actual.y == doctest::Approx(expected.y).epsilon(EPS));
        CHECK(actual.z == doctest::Approx(expected.z).epsilon(EPS));
    }

    std::shared_ptr<BezierCurve> makeCurve() {
        return std::make_shared<BezierCurve>(
            std::vector<glm::vec3>{
                {0.0f, 0.0f, 0.0f},
                {1.0f, 2.0f, 0.0f},
                {3.0f, 2.0f, 0.0f},
                {4.0f, 0.0f, 0.0f}
            }
        );
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


TEST_CASE("Rotation builder builds rotation spec with configured values") {
    const AnimationSpec spec = Rotation(Animation::LOOP)
        .duration(2.5f)
        .rotationAxis(glm::vec3(0.0f, 2.0f, 0.0f))
        .totalAngle(180.0f)
        .buildSpec();

    REQUIRE(std::holds_alternative<RotationAnimationSpec>(spec.data));
    const auto& data = std::get<RotationAnimationSpec>(spec.data);

    CHECK(data.mode == Animation::LOOP);
    REQUIRE(data.duration.has_value());
    REQUIRE(data.rotationAxis.has_value());
    REQUIRE(data.totalAngle.has_value());

    CHECK(*data.duration == doctest::Approx(2.5f));
    checkVec3Approx(*data.rotationAxis, glm::vec3(0.0f, 2.0f, 0.0f));
    CHECK(*data.totalAngle == doctest::Approx(180.0f));
}

TEST_CASE("Orbiting builder builds orbiting spec with configured values") {
    const AnimationSpec spec = Orbiting(Animation::BOUNCE)
        .duration(3.0f)
        .radius(5.0f)
        .rotationAxis(glm::vec3(0.0f, 1.0f, 0.0f))
        .totalAngle(270.0f)
        .buildSpec();

    REQUIRE(std::holds_alternative<OrbitingAnimationSpec>(spec.data));
    const auto& data = std::get<OrbitingAnimationSpec>(spec.data);

    CHECK(data.mode == Animation::BOUNCE);
    REQUIRE(data.duration.has_value());
    REQUIRE(data.radius.has_value());
    REQUIRE(data.rotationAxis.has_value());
    REQUIRE(data.totalAngle.has_value());

    CHECK(*data.duration == doctest::Approx(3.0f));
    CHECK(*data.radius == doctest::Approx(5.0f));
    checkVec3Approx(*data.rotationAxis, glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(*data.totalAngle == doctest::Approx(270.0f));
}

TEST_CASE("Scaling builder supports scalar and vector target scale") {
    SUBCASE("scalar target scale") {
        const AnimationSpec spec = Scaling(Animation::ONCE)
            .duration(1.5f)
            .targetScale(2.0f)
            .buildSpec();

        REQUIRE(std::holds_alternative<ScalingAnimationSpec>(spec.data));
        const auto& data = std::get<ScalingAnimationSpec>(spec.data);

        CHECK(data.mode == Animation::ONCE);
        REQUIRE(data.duration.has_value());
        REQUIRE(data.targetScale.has_value());

        CHECK(*data.duration == doctest::Approx(1.5f));
        checkVec3Approx(*data.targetScale, glm::vec3(2.0f, 2.0f, 2.0f));
    }

    SUBCASE("vector target scale") {
        const AnimationSpec spec = Scaling(Animation::ONCE)
            .targetScale(glm::vec3(2.0f, 3.0f, 4.0f))
            .buildSpec();

        REQUIRE(std::holds_alternative<ScalingAnimationSpec>(spec.data));
        const auto& data = std::get<ScalingAnimationSpec>(spec.data);

        REQUIRE(data.targetScale.has_value());
        checkVec3Approx(*data.targetScale, glm::vec3(2.0f, 3.0f, 4.0f));
    }
}

TEST_CASE("Translation builder builds translation spec with configured values") {
    const AnimationSpec spec = Translation(Animation::LOOP)
        .duration(4.0f)
        .direction(glm::vec3(0.0f, 0.0f, 2.0f))
        .totalDistance(10.0f)
        .buildSpec();

    REQUIRE(std::holds_alternative<TranslationAnimationSpec>(spec.data));
    const auto& data = std::get<TranslationAnimationSpec>(spec.data);

    CHECK(data.mode == Animation::LOOP);
    REQUIRE(data.duration.has_value());
    REQUIRE(data.direction.has_value());
    REQUIRE(data.totalDistance.has_value());

    CHECK(*data.duration == doctest::Approx(4.0f));
    checkVec3Approx(*data.direction, glm::vec3(0.0f, 0.0f, 2.0f));
    CHECK(*data.totalDistance == doctest::Approx(10.0f));
}

TEST_CASE("KeyframeAnimation builder collects keyframes") {
    const auto a = makeTransform(glm::vec3(1.0f, 0.0f, 0.0f));
    const auto b = makeTransform(glm::vec3(2.0f, 3.0f, 4.0f));

    const AnimationSpec spec = KeyframeAnimation(Animation::ONCE)
        .duration(5.0f)
        .keyframe(1.0f, a)
        .keyframe({3.0f, b})
        .buildSpec();

    REQUIRE(std::holds_alternative<KeyframeAnimationSpec>(spec.data));
    const auto& data = std::get<KeyframeAnimationSpec>(spec.data);

    CHECK(data.mode == Animation::ONCE);
    REQUIRE(data.duration.has_value());
    CHECK(*data.duration == doctest::Approx(5.0f));

    REQUIRE(data.keyframes.size() == 2);
    CHECK(data.keyframes[0].timestamp == doctest::Approx(1.0f));
    CHECK(data.keyframes[1].timestamp == doctest::Approx(3.0f));
    checkVec3Approx(data.keyframes[0].transform.getTranslation(), glm::vec3(1.0f, 0.0f, 0.0f));
    checkVec3Approx(data.keyframes[1].transform.getTranslation(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("CurveAnimation builder stores curve and duration") {
    const auto curve = makeCurve();

    const AnimationSpec spec = CurveAnimation(Animation::BOUNCE)
        .duration(2.0f)
        .curve(curve)
        .buildSpec();

    REQUIRE(std::holds_alternative<CurveAnimationSpec>(spec.data));
    const auto& data = std::get<CurveAnimationSpec>(spec.data);

    CHECK(data.mode == Animation::BOUNCE);
    REQUIRE(data.duration.has_value());
    CHECK(*data.duration == doctest::Approx(2.0f));
    CHECK(data.curve == curve);
}

TEST_CASE("Animation compiler builds Rotation animation from spec") {
    const auto spec = Rotation(Animation::LOOP)
        .duration(2.5f)
        .rotationAxis(glm::vec3(0.0f, 2.0f, 0.0f))
        .totalAngle(180.0f)
        .buildSpec();

    const auto animation = buildAnimation(spec);
    REQUIRE(animation != nullptr);

    const auto rotation = std::dynamic_pointer_cast<pixelforge::scene::animation::Rotation>(animation);
    REQUIRE(rotation != nullptr);

    CHECK(rotation->getMode() == Animation::LOOP);
    CHECK(rotation->getDuration() == doctest::Approx(2.5f));
    checkVec3Approx(rotation->getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(rotation->getTotalAngle() == doctest::Approx(180.0f));
}

TEST_CASE("Animation compiler builds Orbiting animation from spec") {
    const auto spec = Orbiting(Animation::ONCE)
        .duration(3.0f)
        .radius(5.0f)
        .rotationAxis(glm::vec3(0.0f, 3.0f, 0.0f))
        .totalAngle(270.0f)
        .buildSpec();

    const auto animation = buildAnimation(spec);
    REQUIRE(animation != nullptr);

    const auto orbiting = std::dynamic_pointer_cast<pixelforge::scene::animation::Orbiting>(animation);
    REQUIRE(orbiting != nullptr);

    CHECK(orbiting->getMode() == Animation::ONCE);
    CHECK(orbiting->getDuration() == doctest::Approx(3.0f));
    CHECK(orbiting->getRadius() == doctest::Approx(5.0f));
    checkVec3Approx(orbiting->getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
    CHECK(orbiting->getTotalAngle() == doctest::Approx(270.0f));
}

TEST_CASE("Animation compiler builds Scaling animation from spec") {
    const auto spec = Scaling(Animation::BOUNCE)
        .duration(4.0f)
        .targetScale(glm::vec3(2.0f, 3.0f, 4.0f))
        .buildSpec();

    const auto animation = buildAnimation(spec);
    REQUIRE(animation != nullptr);

    const auto scaling = std::dynamic_pointer_cast<pixelforge::scene::animation::Scaling>(animation);
    REQUIRE(scaling != nullptr);

    CHECK(scaling->getMode() == Animation::BOUNCE);
    CHECK(scaling->getDuration() == doctest::Approx(4.0f));
    checkVec3Approx(scaling->getTargetScale(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("Animation compiler builds Translation animation from spec") {
    const auto spec = Translation(Animation::LOOP)
        .duration(1.5f)
        .direction(glm::vec3(0.0f, 0.0f, 4.0f))
        .totalDistance(8.0f)
        .buildSpec();

    const auto animation = buildAnimation(spec);
    REQUIRE(animation != nullptr);

    const auto translation = std::dynamic_pointer_cast<pixelforge::scene::animation::Translation>(animation);
    REQUIRE(translation != nullptr);

    CHECK(translation->getMode() == Animation::LOOP);
    CHECK(translation->getDuration() == doctest::Approx(1.5f));
    checkVec3Approx(translation->getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
    CHECK(translation->getTotalDistance() == doctest::Approx(8.0f));
}

TEST_CASE("Animation compiler builds KeyframeAnimation from spec") {
    const auto a = makeTransform(glm::vec3(1.0f, 0.0f, 0.0f));
    const auto b = makeTransform(glm::vec3(2.0f, 3.0f, 4.0f));

    const auto spec = KeyframeAnimation(Animation::ONCE)
        .duration(5.0f)
        .keyframe(1.0f, a)
        .keyframe(3.0f, b)
        .buildSpec();

    const auto animation = buildAnimation(spec);
    REQUIRE(animation != nullptr);

    const auto keyframeAnimation = std::dynamic_pointer_cast<pixelforge::scene::animation::KeyframeAnimation>(animation);
    REQUIRE(keyframeAnimation != nullptr);

    CHECK(keyframeAnimation->getMode() == Animation::ONCE);
    CHECK(keyframeAnimation->getDuration() == doctest::Approx(5.0f));

    const auto& keyframes = keyframeAnimation->getKeyframes();
    REQUIRE(keyframes.size() == 2);
    CHECK(keyframes[0].timestamp == doctest::Approx(1.0f));
    CHECK(keyframes[1].timestamp == doctest::Approx(3.0f));
    checkVec3Approx(keyframes[0].transform.getTranslation(), glm::vec3(1.0f, 0.0f, 0.0f));
    checkVec3Approx(keyframes[1].transform.getTranslation(), glm::vec3(2.0f, 3.0f, 4.0f));
}

TEST_CASE("Animation compiler builds CurveAnimation from spec") {
    const auto curve = makeCurve();

    const auto spec = CurveAnimation(Animation::BOUNCE)
        .duration(2.0f)
        .curve(curve)
        .buildSpec();

    const auto animation = buildAnimation(spec);
    REQUIRE(animation != nullptr);

    const auto curveAnimation = std::dynamic_pointer_cast<pixelforge::scene::animation::CurveAnimation>(animation);
    REQUIRE(curveAnimation != nullptr);

    CHECK(curveAnimation->getMode() == Animation::BOUNCE);
    CHECK(curveAnimation->getDuration() == doctest::Approx(2.0f));
    CHECK(curveAnimation->getCurve() == curve);
}

TEST_CASE("Animation compiler leaves unspecified values at runtime defaults") {
    SUBCASE("rotation defaults") {
        const AnimationSpec spec = Rotation(Animation::ONCE).buildSpec();
        const auto animation = buildAnimation(spec);
        const auto rotation = std::dynamic_pointer_cast<pixelforge::scene::animation::Rotation>(animation);
        REQUIRE(rotation != nullptr);

        CHECK(rotation->getMode() == Animation::ONCE);
        CHECK(rotation->getDuration() == doctest::Approx(1.0f));
        checkVec3Approx(rotation->getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
        CHECK(rotation->getTotalAngle() == doctest::Approx(360.0f));
    }

    SUBCASE("orbiting defaults") {
        const AnimationSpec spec = Orbiting(Animation::LOOP).buildSpec();
        const auto animation = buildAnimation(spec);
        const auto orbiting = std::dynamic_pointer_cast<pixelforge::scene::animation::Orbiting>(animation);
        REQUIRE(orbiting != nullptr);

        CHECK(orbiting->getMode() == Animation::LOOP);
        CHECK(orbiting->getDuration() == doctest::Approx(1.0f));
        CHECK(orbiting->getRadius() == doctest::Approx(0.0f));
        checkVec3Approx(orbiting->getRotationAxis(), glm::vec3(0.0f, 1.0f, 0.0f));
        CHECK(orbiting->getTotalAngle() == doctest::Approx(360.0f));
    }

    SUBCASE("scaling defaults") {
        const AnimationSpec spec = Scaling(Animation::ONCE).buildSpec();
        const auto animation = buildAnimation(spec);
        const auto scaling = std::dynamic_pointer_cast<pixelforge::scene::animation::Scaling>(animation);
        REQUIRE(scaling != nullptr);

        CHECK(scaling->getMode() == Animation::ONCE);
        CHECK(scaling->getDuration() == doctest::Approx(1.0f));
        checkVec3Approx(scaling->getTargetScale(), glm::vec3(1.0f));
    }

    SUBCASE("translation defaults") {
        const AnimationSpec spec = Translation(Animation::ONCE).buildSpec();
        const auto animation = buildAnimation(spec);
        const auto translation = std::dynamic_pointer_cast<pixelforge::scene::animation::Translation>(animation);
        REQUIRE(translation != nullptr);

        CHECK(translation->getMode() == Animation::ONCE);
        CHECK(translation->getDuration() == doctest::Approx(1.0f));
        checkVec3Approx(translation->getDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
        CHECK(translation->getTotalDistance() == doctest::Approx(1.0f));
    }

    SUBCASE("keyframe defaults") {
        const AnimationSpec spec = KeyframeAnimation(Animation::ONCE).buildSpec();
        const auto animation = buildAnimation(spec);
        const auto keyframeAnimation = std::dynamic_pointer_cast<pixelforge::scene::animation::KeyframeAnimation>(animation);
        REQUIRE(keyframeAnimation != nullptr);

        CHECK(keyframeAnimation->getMode() == Animation::ONCE);
        CHECK(keyframeAnimation->getDuration() == doctest::Approx(0.01f));
        CHECK(keyframeAnimation->getKeyframes().empty());
    }

    SUBCASE("curve animation defaults") {
        const AnimationSpec spec = CurveAnimation(Animation::BOUNCE).buildSpec();
        const auto animation = buildAnimation(spec);
        const auto curveAnimation = std::dynamic_pointer_cast<pixelforge::scene::animation::CurveAnimation>(animation);
        REQUIRE(curveAnimation != nullptr);

        CHECK(curveAnimation->getMode() == Animation::BOUNCE);
        CHECK(curveAnimation->getDuration() == doctest::Approx(1.0f));
        CHECK(curveAnimation->getCurve() == nullptr);
    }
}
