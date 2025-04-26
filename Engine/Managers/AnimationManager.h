#pragma once
#include "InstanceManager.h"
#include "CurveManager.h"
#include "../Scene/Animations/Animation.h"
#include "../Scene/Animations/CurveAnimation.h"
#include "../Scene/Animations/KeyframeAnimation.h"
#include "../Scene/Animations/Orbiting.h"
#include "../Scene/Animations/Rotation.h"
#include "../Scene/Animations/Scaling.h"
#include "../Scene/Animations/Translation.h"


class AnimationManager final : public InstanceManager<Animation, AnimationManager> {
public:
    static size_t Hash(const Animation& animation) {
        size_t seed = typeid(animation).hash_code();
        CombineHashes(seed, hash<int>{}(animation.GetMode()));
        CombineHashes(seed, hash<float>{}(animation.GetDuration()));
        if (auto* curveAnimation = dynamic_cast<const CurveAnimation*>(&animation)) {
            CombineHashes(seed, HashAnimation(*curveAnimation));
        } if (auto* keyframeAnimation = dynamic_cast<const KeyframeAnimation*>(&animation)) {
            CombineHashes(seed, HashAnimation(*keyframeAnimation));
        } else if (auto* orbiting = dynamic_cast<const Orbiting*>(&animation)) {
            CombineHashes(seed, HashAnimation(*orbiting));
        } else if (auto* rotation = dynamic_cast<const Rotation*>(&animation)) {
            CombineHashes(seed, HashAnimation(*rotation));
        } else if (auto* scaling = dynamic_cast<const Scaling*>(&animation)) {
            CombineHashes(seed, HashAnimation(*scaling));
        } else if (auto* translation = dynamic_cast<const Translation*>(&animation)) {
            CombineHashes(seed, HashAnimation(*translation));
        }
        return seed;
    }

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
        if (auto* leftRotation = dynamic_cast<const Rotation*>(&left)) {
            auto* rightRotation = dynamic_cast<const Rotation*>(&right);
            if (!rightRotation) return false;
            return leftRotation->GetTotalAngle() == rightRotation->GetTotalAngle() &&
                leftRotation->GetRotationAxis() == rightRotation->GetRotationAxis();
        }
        if (auto* leftScaling = dynamic_cast<const Scaling*>(&left)) {
            auto* rightScaling = dynamic_cast<const Scaling*>(&right);
            if (!rightScaling) return false;
            return leftScaling->GetTargetScale() == rightScaling->GetTargetScale();
        }
        if (auto* leftTranslation = dynamic_cast<const Translation*>(&left)) {
            auto* rightTranslation = dynamic_cast<const Translation*>(&right);
            if (!rightTranslation) return false;
            return leftTranslation->GetTotalDistance() == rightTranslation->GetTotalDistance() &&
                leftTranslation->GetDirection() == rightTranslation->GetDirection();
        }
        return false;
    }

private:
    static size_t HashAnimation(const CurveAnimation& animation) {
        size_t seed = 0;
        CombineHashes(seed, CurveManager::Hash(*animation.GetCurve()));
        return seed;
    }

    static size_t HashAnimation(const KeyframeAnimation& animation) {
        size_t seed = 0;
        for (const auto& [timeStamp, transform] : animation.GetKeyframes()) {
            CombineHashes(seed, hash<float>()(timeStamp));
            CombineHashes(seed, hash<vec3>()(transform.translation));
            CombineHashes(seed, hash<quat>()(transform.rotation));
            CombineHashes(seed, hash<vec3>{}(transform.scaling));
        }
        return seed;
    }

    static size_t HashAnimation(const Orbiting& animation) {
        size_t seed = 0;
        CombineHashes(seed, hash<float>()(animation.GetTotalAngle()));
        CombineHashes(seed, hash<vec3>()(animation.GetRotationAxis()));
        CombineHashes(seed, hash<float>()(animation.GetRadius()));
        return seed;
    }

    static size_t HashAnimation(const Rotation& animation) {
        size_t seed = 0;
        CombineHashes(seed, hash<float>()(animation.GetTotalAngle()));
        CombineHashes(seed, hash<vec3>()(animation.GetRotationAxis()));
        return seed;
    }

    static size_t HashAnimation(const Scaling& animation) {
        size_t seed = 0;
        CombineHashes(seed, hash<vec3>()(animation.GetTargetScale()));
        return seed;
    }

    static size_t HashAnimation(const Translation& animation) {
        size_t seed = 0;
        CombineHashes(seed, hash<float>()(animation.GetTotalDistance()));
        CombineHashes(seed, hash<vec3>()(animation.GetDirection()));
        return seed;
    }
};
