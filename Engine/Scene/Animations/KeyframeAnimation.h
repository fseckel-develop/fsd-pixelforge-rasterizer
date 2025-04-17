#pragma once
#include "Animation.h"
#include "../Transforms/Transform.h"
#include <vector>
using namespace std;


typedef struct Keyframe {
    float timeStamp = 0.0f;
    Transform transform;
} KeyFrame;


class KeyframeAnimation final : public Animation {
public:
    explicit KeyframeAnimation(Mode = ONCE);
    void AddKeyframe(float, const Transform&);
    Transform GetOffset(float) override;

private:
    vector<Keyframe> keyframes;
    [[nodiscard]] Transform Interpolate(float) const;
};
