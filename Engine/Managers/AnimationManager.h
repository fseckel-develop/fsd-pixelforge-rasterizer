#pragma once
#include "InstanceManager.h"
#include "CurveManager.h"
#include "../Scene/Animations/CurveAnimation.h"
#include "../Scene/Animations/KeyframeAnimation.h"
#include "../Scene/Animations/Orbiting.h"
#include "../Scene/Animations/Rotating.h"
#include "../Scene/Animations/Scaling.h"
#include "../Scene/Animations/Translating.h"


/// Extension of the InstanceManager class for managing
/// animation instances and ensuring their uniqueness.
class AnimationManager final : public InstanceManager<Animation, AnimationManager> {
public:
    /// Computes the hash for an animation instance.
    /// @param animation The animation instance to hash.
    /// @return Hash value for the animation.
    static size_t Hash(const Animation& animation) {
        size_t seed = typeid(animation).hash_code();
        CombineHashes(seed, std::hash<int>{}(animation.GetMode()));
        CombineHashes(seed, std::hash<float>{}(animation.GetDuration()));
        if (auto* curveAnimation = dynamic_cast<const CurveAnimation*>(&animation)) {
            CombineHashes(seed, HashAnimation(*curveAnimation));
        } else if (auto* keyframeAnimation = dynamic_cast<const KeyframeAnimation*>(&animation)) {
            CombineHashes(seed, HashAnimation(*keyframeAnimation));
        } else if (auto* orbiting = dynamic_cast<const Orbiting*>(&animation)) {
            CombineHashes(seed, HashAnimation(*orbiting));
        } else if (auto* rotation = dynamic_cast<const Rotating*>(&animation)) {
            CombineHashes(seed, HashAnimation(*rotation));
        } else if (auto* scaling = dynamic_cast<const Scaling*>(&animation)) {
            CombineHashes(seed, HashAnimation(*scaling));
        } else if (auto* translation = dynamic_cast<const Translating*>(&animation)) {
            CombineHashes(seed, HashAnimation(*translation));
        }
        return seed;
    }

    /// Compares two animation instances for equality.
    /// @param left The first animation to compare.
    /// @param right The second animation to compare.
    /// @return True if the animations are equal, false otherwise.
    static bool Equals(const Animation& left, const Animation& right) {
        if (typeid(left) != typeid(right)) return false;
        if (left.GetMode() != right.GetMode()) return false;
        if (left.GetDuration() != right.GetDuration()) return false;
        if (auto* leftCurve = dynamic_cast<const CurveAnimation*>(&left)) {
            auto* rightCurve = dynamic_cast<const CurveAnimation*>(&right);
            if (!rightCurve) return false;
            return CurveManager::Equals(*leftCurve->GetCurve(), *rightCurve->GetCurve());
        }
        if (auto* leftKeyframe = dynamic_cast<const KeyframeAnimation*>(&left)) {
            auto* rightKeyframe = dynamic_cast<const KeyframeAnimation*>(&right);
            if (!rightKeyframe) return false;
            return leftKeyframe->GetKeyframes() == rightKeyframe->GetKeyframes();
        }
        if (auto* leftOrbiting = dynamic_cast<const Orbiting*>(&left)) {
            auto* rightOrbiting = dynamic_cast<const Orbiting*>(&right);
            if (!rightOrbiting) return false;
            return leftOrbiting->GetTotalAngle() == rightOrbiting->GetTotalAngle() &&
                   leftOrbiting->GetRotationAxis() == rightOrbiting->GetRotationAxis() &&
                   leftOrbiting->GetRadius() == rightOrbiting->GetRadius();
        }
        if (auto* leftRotation = dynamic_cast<const Rotating*>(&left)) {
            auto* rightRotation = dynamic_cast<const Rotating*>(&right);
            if (!rightRotation) return false;
            return leftRotation->GetTotalAngle() == rightRotation->GetTotalAngle() &&
                   leftRotation->GetRotationAxis() == rightRotation->GetRotationAxis();
        }
        if (auto* leftScaling = dynamic_cast<const Scaling*>(&left)) {
            auto* rightScaling = dynamic_cast<const Scaling*>(&right);
            if (!rightScaling) return false;
            return leftScaling->GetTargetScale() == rightScaling->GetTargetScale();
        }
        if (auto* leftTranslation = dynamic_cast<const Translating*>(&left)) {
            auto* rightTranslation = dynamic_cast<const Translating*>(&right);
            if (!rightTranslation) return false;
            return leftTranslation->GetTotalDistance() == rightTranslation->GetTotalDistance() &&
                   leftTranslation->GetDirection() == rightTranslation->GetDirection();
        }
        return false;
    }

private:
    /// Computes the hash for a curve animation instance.
    /// @param animation Curve animation instance to hash.
    /// @return The computed hash value for the curve animation instance.
    static size_t HashAnimation(const CurveAnimation& animation) {
        size_t seed = 0;
        CombineHashes(seed, CurveManager::Hash(*animation.GetCurve()));
        return seed;
    }

    /// Computes the hash for a keyframe animation instance.
    /// @param animation Keyframe animation instance to hash.
    /// @return The computed hash value for the keyframe animation instance.
    static size_t HashAnimation(const KeyframeAnimation& animation) {
        size_t seed = 0;
        for (const auto& [timeStamp, transform] : animation.GetKeyframes()) {
            CombineHashes(seed, std::hash<float>()(timeStamp));
            CombineHashes(seed, std::hash<glm::vec3>()(transform.GetTranslation()));
            CombineHashes(seed, std::hash<glm::quat>()(transform.GetRotation()));
            CombineHashes(seed, std::hash<glm::vec3>{}(transform.GetScale()));
        }
        return seed;
    }

    /// Computes the hash for an orbiting animation instance.
    /// @param animation Orbiting animation instance to hash.
    /// @return The computed hash value for the orbiting animation instance.
    static size_t HashAnimation(const Orbiting& animation) {
        size_t seed = 0;
        CombineHashes(seed, std::hash<float>()(animation.GetTotalAngle()));
        CombineHashes(seed, std::hash<glm::vec3>()(animation.GetRotationAxis()));
        CombineHashes(seed, std::hash<float>()(animation.GetRadius()));
        return seed;
    }

    /// Computes the hash for a rotating animation instance.
    /// @param animation Rotating animation instance to hash.
    /// @return The computed hash value for the rotating animation instance.
    static size_t HashAnimation(const Rotating& animation) {
        size_t seed = 0;
        CombineHashes(seed, std::hash<float>()(animation.GetTotalAngle()));
        CombineHashes(seed, std::hash<glm::vec3>()(animation.GetRotationAxis()));
        return seed;
    }

    /// Computes the hash for a scaling animation instance.
    /// @param animation Scaling animation instance to hash.
    /// @return The computed hash value for the scaling animation instance.
    static size_t HashAnimation(const Scaling& animation) {
        size_t seed = 0;
        CombineHashes(seed, std::hash<glm::vec3>()(animation.GetTargetScale()));
        return seed;
    }

    /// Computes the hash for a translating animation instance.
    /// @param animation Translating animation instance to hash.
    /// @return The computed hash value for the translating animation instance.
    static size_t HashAnimation(const Translating& animation) {
        size_t seed = 0;
        CombineHashes(seed, std::hash<float>()(animation.GetTotalDistance()));
        CombineHashes(seed, std::hash<glm::vec3>()(animation.GetDirection()));
        return seed;
    }
};
