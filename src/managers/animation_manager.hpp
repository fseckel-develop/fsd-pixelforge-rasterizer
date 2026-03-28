#pragma once
#include <pixelforge/scene/animation/curve_animation.hpp>
#include <pixelforge/scene/animation/elliptic_orbiting.hpp>
#include <pixelforge/scene/animation/keyframe_animation.hpp>
#include <pixelforge/scene/animation/orbiting.hpp>
#include <pixelforge/scene/animation/rotation.hpp>
#include <pixelforge/scene/animation/scaling.hpp>
#include <pixelforge/scene/animation/translation.hpp>
#include "managers/instance_manager.hpp"
#include "managers/curve_manager.hpp"


namespace pixelforge::management {

    /// Extension of the InstanceManager class for managing
    /// animation instances and ensuring their uniqueness.
    class AnimationManager final : public InstanceManager<scene::animation::Animation, AnimationManager> {
    public:
        /// Computes the hash for an animation instance.
        /// @param animation The animation instance to hash.
        /// @return Hash value for the animation.
        static size_t hash(const scene::animation::Animation& animation) {
            size_t seed = typeid(animation).hash_code();
            combineHashes(seed, std::hash<int>{}(animation.getMode()));
            combineHashes(seed, std::hash<float>{}(animation.getDuration()));
            if (auto* curveAnimation = dynamic_cast<const scene::animation::CurveAnimation*>(&animation)) {
                combineHashes(seed, hashAnimation(*curveAnimation));
            } else if (auto* keyframeAnimation = dynamic_cast<const scene::animation::KeyframeAnimation*>(&animation)) {
                combineHashes(seed, hashAnimation(*keyframeAnimation));
            } else if (auto* orbiting = dynamic_cast<const scene::animation::Orbiting*>(&animation)) {
                combineHashes(seed, hashAnimation(*orbiting));
            } else if (auto* ellipticOrbiting = dynamic_cast<const scene::animation::EllipticOrbiting*>(&animation)) {
                combineHashes(seed, hashAnimation(*ellipticOrbiting));
            } else if (auto* rotation = dynamic_cast<const scene::animation::Rotation*>(&animation)) {
                combineHashes(seed, hashAnimation(*rotation));
            } else if (auto* scaling = dynamic_cast<const scene::animation::Scaling*>(&animation)) {
                combineHashes(seed, hashAnimation(*scaling));
            } else if (auto* translation = dynamic_cast<const scene::animation::Translation*>(&animation)) {
                combineHashes(seed, hashAnimation(*translation));
            }
            return seed;
        }

        /// Compares two animation instances for equality.
        /// @param left The first animation to compare.
        /// @param right The second animation to compare.
        /// @return True if the animations are equal, false otherwise.
        static bool equals(const scene::animation::Animation& left, const scene::animation::Animation& right) {
            if (typeid(left) != typeid(right)) return false;
            if (left.getMode() != right.getMode()) return false;
            if (left.getDuration() != right.getDuration()) return false;
            if (auto* leftCurve = dynamic_cast<const scene::animation::CurveAnimation*>(&left)) {
                auto* rightCurve = dynamic_cast<const scene::animation::CurveAnimation*>(&right);
                if (!rightCurve) return false;
                return CurveManager::equals(*leftCurve->getCurve(), *rightCurve->getCurve());
            }
            if (auto* leftKeyframe = dynamic_cast<const scene::animation::KeyframeAnimation*>(&left)) {
                auto* rightKeyframe = dynamic_cast<const scene::animation::KeyframeAnimation*>(&right);
                if (!rightKeyframe) return false;
                return leftKeyframe->getKeyframes() == rightKeyframe->getKeyframes();
            }
            if (auto* leftOrbiting = dynamic_cast<const scene::animation::Orbiting*>(&left)) {
                auto* rightOrbiting = dynamic_cast<const scene::animation::Orbiting*>(&right);
                if (!rightOrbiting) return false;
                return leftOrbiting->getTotalAngle() == rightOrbiting->getTotalAngle() &&
                       leftOrbiting->getRotationAxis() == rightOrbiting->getRotationAxis() &&
                       leftOrbiting->getRadius() == rightOrbiting->getRadius();
            }
            if (auto* leftEllipticOrbiting = dynamic_cast<const scene::animation::EllipticOrbiting*>(&left)) {
                auto* rightEllipticOrbiting = dynamic_cast<const scene::animation::EllipticOrbiting*>(&right);
                if (!rightEllipticOrbiting) return false;
                return leftEllipticOrbiting->getTotalAngle() == rightEllipticOrbiting->getTotalAngle() &&
                       leftEllipticOrbiting->getRotationAxis() == rightEllipticOrbiting->getRotationAxis() &&
                       leftEllipticOrbiting->getSemiMajorAxis() == rightEllipticOrbiting->getSemiMajorAxis() &&
                       leftEllipticOrbiting->getSemiMinorAxis() == rightEllipticOrbiting->getSemiMinorAxis() &&
                       leftEllipticOrbiting->getUseFocusOrigin() == rightEllipticOrbiting->getUseFocusOrigin();
            }
            if (auto* leftRotation = dynamic_cast<const scene::animation::Rotation*>(&left)) {
                auto* rightRotation = dynamic_cast<const scene::animation::Rotation*>(&right);
                if (!rightRotation) return false;
                return leftRotation->getTotalAngle() == rightRotation->getTotalAngle() &&
                       leftRotation->getRotationAxis() == rightRotation->getRotationAxis();
            }
            if (auto* leftScaling = dynamic_cast<const scene::animation::Scaling*>(&left)) {
                auto* rightScaling = dynamic_cast<const scene::animation::Scaling*>(&right);
                if (!rightScaling) return false;
                return leftScaling->getTargetScale() == rightScaling->getTargetScale();
            }
            if (auto* leftTranslation = dynamic_cast<const scene::animation::Translation*>(&left)) {
                auto* rightTranslation = dynamic_cast<const scene::animation::Translation*>(&right);
                if (!rightTranslation) return false;
                return leftTranslation->getTotalDistance() == rightTranslation->getTotalDistance() &&
                       leftTranslation->getDirection() == rightTranslation->getDirection();
            }
            return false;
        }

    private:
        /// Computes the hash for a curve animation instance.
        /// @param animation Curve animation instance to hash.
        /// @return The computed hash value for the curve animation instance.
        static size_t hashAnimation(const scene::animation::CurveAnimation& animation) {
            size_t seed = 0;
            combineHashes(seed, CurveManager::hash(*animation.getCurve()));
            return seed;
        }

        /// Computes the hash for a keyframe animation instance.
        /// @param animation Keyframe animation instance to hash.
        /// @return The computed hash value for the keyframe animation instance.
        static size_t hashAnimation(const scene::animation::KeyframeAnimation& animation) {
            size_t seed = 0;
            for (const auto& [timeStamp, transform] : animation.getKeyframes()) {
                combineHashes(seed, std::hash<float>()(timeStamp));
                combineHashes(seed, std::hash<glm::vec3>()(transform.getTranslation()));
                combineHashes(seed, std::hash<glm::quat>()(transform.getRotation()));
                combineHashes(seed, std::hash<glm::vec3>{}(transform.getScale()));
            }
            return seed;
        }

        /// Computes the hash for an orbiting animation instance.
        /// @param animation Orbiting animation instance to hash.
        /// @return The computed hash value for the orbiting animation instance.
        static size_t hashAnimation(const scene::animation::Orbiting& animation) {
            size_t seed = 0;
            combineHashes(seed, std::hash<float>()(animation.getTotalAngle()));
            combineHashes(seed, std::hash<glm::vec3>()(animation.getRotationAxis()));
            combineHashes(seed, std::hash<float>()(animation.getRadius()));
            return seed;
        }

        /// Computes the hash for an elliptic orbiting animation instance.
        /// @param animation Elliptic orbiting animation instance to hash.
        /// @return The computed hash value for the elliptic orbiting animation instance.
        static size_t hashAnimation(const scene::animation::EllipticOrbiting& animation) {
            size_t seed = 0;
            combineHashes(seed, std::hash<float>()(animation.getTotalAngle()));
            combineHashes(seed, std::hash<glm::vec3>()(animation.getRotationAxis()));
            combineHashes(seed, std::hash<float>()(animation.getSemiMajorAxis()));
            combineHashes(seed, std::hash<float>()(animation.getSemiMinorAxis()));
            combineHashes(seed, std::hash<bool>()(animation.getUseFocusOrigin()));
            return seed;
        }

        /// Computes the hash for a rotating animation instance.
        /// @param animation Rotating animation instance to hash.
        /// @return The computed hash value for the rotating animation instance.
        static size_t hashAnimation(const scene::animation::Rotation& animation) {
            size_t seed = 0;
            combineHashes(seed, std::hash<float>()(animation.getTotalAngle()));
            combineHashes(seed, std::hash<glm::vec3>()(animation.getRotationAxis()));
            return seed;
        }

        /// Computes the hash for a scaling animation instance.
        /// @param animation Scaling animation instance to hash.
        /// @return The computed hash value for the scaling animation instance.
        static size_t hashAnimation(const scene::animation::Scaling& animation) {
            size_t seed = 0;
            combineHashes(seed, std::hash<glm::vec3>()(animation.getTargetScale()));
            return seed;
        }

        /// Computes the hash for a translating animation instance.
        /// @param animation Translating animation instance to hash.
        /// @return The computed hash value for the translating animation instance.
        static size_t hashAnimation(const scene::animation::Translation& animation) {
            size_t seed = 0;
            combineHashes(seed, std::hash<float>()(animation.getTotalDistance()));
            combineHashes(seed, std::hash<glm::vec3>()(animation.getDirection()));
            return seed;
        }
    };

} // namespace pixelforge::management
