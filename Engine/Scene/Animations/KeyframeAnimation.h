#pragma once
#include "Animation.h"
#include "../Transforms/Transform.h"
#include <vector>
using namespace std;


typedef struct Keyframe {
    float timeStamp = 0.0f;
    Transform transform;
    bool operator==(const Keyframe& other) const {
        return timeStamp == other.timeStamp &&
            transform.translation == other.transform.translation &&
            transform.rotation == other.transform.rotation &&
            transform.scaling == other.transform.scaling;
    }
} Keyframe;


class KeyframeAnimation final : public Animation {
public:
    explicit KeyframeAnimation(Mode = ONCE);
    void AddKeyframe(float, const Transform&);
    [[nodiscard]] const vector<Keyframe>& GetKeyframes() const;
    [[nodiscard]] Transform GetOffset() override;

private:
    vector<Keyframe> keyframes;
    [[nodiscard]] Transform Interpolate(float) const;
};
