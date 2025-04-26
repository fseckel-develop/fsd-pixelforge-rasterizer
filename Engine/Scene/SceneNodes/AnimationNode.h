#pragma once
#include <vector>
#include "TransformNode.h"
class Animation;
using namespace std;


class AnimationNode final : public TransformNode {
public:
    explicit AnimationNode(const string&);
    void AddAnimation(const shared_ptr<Animation>&) override;
    void RemoveAnimation(const shared_ptr<Animation>&);
    void PlayAllAnimations() const;
    void PauseAllAnimations() const;
    void ResumeAllAnimations() const;
    void StopAllAnimations() const;
    void ResetAllAnimations() const;
    void UpdateAllAnimations(float);

protected:
    void UpdateSelf(float) override;

private:
    std::vector<shared_ptr<Animation>> animations;
    void ForEachAnimation(void (Animation::*action)()) const;
};
