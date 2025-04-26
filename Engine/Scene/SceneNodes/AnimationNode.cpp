#include "AnimationNode.h"
#include "../../Managers/AnimationManager.h"


AnimationNode::AnimationNode(const string& name):
    TransformNode(name) {
}


void AnimationNode::AddAnimation(const shared_ptr<Animation>& animation) {
    if (animation) {
        animation->Reset();
        animations.push_back(AnimationManager::GetOrCreate(animation));
        MarkGlobalTransformDirty();
    }
}


void AnimationNode::RemoveAnimation(const shared_ptr<Animation>& animation) {
    erase(animations, animation);
    MarkGlobalTransformDirty();
}


void AnimationNode::PlayAllAnimations() const {
    ForEachAnimation(&Animation::Play);
}


void AnimationNode::PauseAllAnimations() const {
    ForEachAnimation(&Animation::Pause);
}


void AnimationNode::ResumeAllAnimations() const {
    ForEachAnimation(&Animation::Resume);
}


void AnimationNode::StopAllAnimations() const {
    ForEachAnimation(&Animation::Stop);
}


void AnimationNode::ResetAllAnimations() const {
    ForEachAnimation(&Animation::Reset);
}


void AnimationNode::UpdateAllAnimations(const float deltaTime) {
    for (const auto& animation : animations) {
        animation->Update(deltaTime);
    }
    MarkGlobalTransformDirty();
}


void AnimationNode::UpdateSelf(const float deltaTime) {
    UpdateAllAnimations(deltaTime);
    Transform combinedAnimationOffset;
    for (const auto& animation : animations) {
        combinedAnimationOffset = combinedAnimationOffset * animation->GetOffset();
    }
    Transform parentTransform;
    if (const auto transformNode = FindNextTransformAncestorFrom(shared_from_this())) {
        parentTransform = transformNode->GetGlobalTransform();
    }
    globalTransform = parentTransform * localTransform * combinedAnimationOffset;
    globalTransformDirty = false;
}


void AnimationNode::ForEachAnimation(void (Animation::*action)()) const {
    for (const auto& animation : animations) {
        (animation.get()->*action)();
    }
}
