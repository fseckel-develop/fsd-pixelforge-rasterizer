#include <pixelforge/scene/animation/elliptic_orbiting.hpp>
#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include "GLM/ext/quaternion_trigonometric.hpp"


namespace pixelforge::scene::animation {

    using glm::vec3;
    using glm::quat;
    using glm::angleAxis;
    using glm::radians;
    using transform::Transform;


    EllipticOrbiting::EllipticOrbiting(const Mode mode):
        Rotation(mode),
        semiMajorAxis_(1.0f),
        semiMinorAxis_(0.5f) {
    }


    EllipticOrbiting::EllipticOrbiting(
        const Mode mode,
        const float duration,
        const float semiMajorAxis,
        const float semiMinorAxis,
        const vec3 rotationAxis,
        const float totalAngle,
        const bool useFocusOrigin
    ):
        Rotation(mode, duration, rotationAxis, totalAngle),
        semiMajorAxis_(semiMajorAxis),
        semiMinorAxis_(semiMinorAxis),
        useFocusOrigin_(useFocusOrigin) {
    }


    void EllipticOrbiting::setSemiMajorAxis(const float semiMajorAxis) {
        semiMajorAxis_ = semiMajorAxis;
    }


    void EllipticOrbiting::setSemiMinorAxis(const float semiMinorAxis) {
        semiMinorAxis_ = semiMinorAxis;
    }


    void EllipticOrbiting::setUseFocusOrigin(bool useFocusOrigin) {
        useFocusOrigin_ = useFocusOrigin;
    }


    void EllipticOrbiting::setAxes(const float semiMajorAxis, const float semiMinorAxis) {
        semiMajorAxis_ = semiMajorAxis;
        semiMinorAxis_ = semiMinorAxis;
    }


    float EllipticOrbiting::getSemiMajorAxis() const {
        return semiMajorAxis_;
    }


    float EllipticOrbiting::getSemiMinorAxis() const {
        return semiMinorAxis_;
    }


    bool EllipticOrbiting::getUseFocusOrigin() const {
        return useFocusOrigin_;
    }


    transform::Transform EllipticOrbiting::getOffset() const {
        Transform offset;
        const float currentAngle = totalAngle_ * getProgress();
        vec3 localPosition(
            semiMajorAxis_ * cos(currentAngle),
            0.0f,
            semiMinorAxis_ * sin(currentAngle)
        );
        if (useFocusOrigin_) {
            localPosition.x -= sqrt(semiMajorAxis_ * semiMajorAxis_ - semiMinorAxis_ * semiMinorAxis_);
        }

        constexpr vec3 defaultPlaneNormal(0.0f, 1.0f, 0.0f);
        const float cosAngle = glm::clamp(glm::dot(defaultPlaneNormal, rotationAxis_), -1.0f, 1.0f);

        quat orientation;
        if (cosAngle > 0.9999f) {
            orientation = quat(1.0f, 0.0f, 0.0f, 0.0f);
        } else if (cosAngle < -0.9999f) {
            orientation = angleAxis(glm::pi<float>(), vec3(1.0f, 0.0f, 0.0f));
        } else {
            const vec3 rotationVector = glm::normalize(glm::cross(defaultPlaneNormal, rotationAxis_));
            const float angle = acos(cosAngle);
            orientation = angleAxis(angle, rotationVector);
        }

        offset.setTranslation(orientation * localPosition);
        return offset;
    }

} // namespace pixelforge::scene::animation
