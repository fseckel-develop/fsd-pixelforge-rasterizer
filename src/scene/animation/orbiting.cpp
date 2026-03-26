#include <pixelforge/scene/animation/orbiting.hpp>
#include <pixelforge/scene/transform/transform.hpp>


namespace pixelforge::scene::animation {

    using transform::Transform;
    using glm::vec3;
    using glm::quat;


    Orbiting::Orbiting(const Mode mode):
        Rotation(mode),
        radius_(0.0f) {
    }


    Orbiting::Orbiting(const Mode mode, const float duration, const float radius, const vec3& axis, const float angle):
        Rotation(mode, duration, axis, angle) {
        this->radius_ = radius;
    }


    void Orbiting::setRadius(const float radius) {
        this->radius_ = radius;
    }


    float Orbiting::getRadius() const {
        return radius_;
    }


    Transform Orbiting::getOffset() const {
        Transform offset;
        const float currentAngle = totalAngle_ * getProgress();
        auto radiant = vec3(1.0f, 0.0f, 0.0f);
        if (abs(dot(radiant, rotationAxis_)) > 0.99f) {
            radiant = vec3(0.0f, 1.0f, 0.0f);
        }
        radiant = normalize(cross(rotationAxis_, radiant)) * radius_;
        const quat rotation = angleAxis(currentAngle, rotationAxis_);
        const vec3 orbitalPosition = rotation * radiant;
        offset.setTranslation(orbitalPosition);
        offset.setRotation(rotation);
        return offset;
    }

} // namespace pixelforge::scene::animation
