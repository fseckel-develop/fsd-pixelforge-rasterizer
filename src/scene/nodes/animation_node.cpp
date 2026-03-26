#include <pixelforge/scene/nodes/animation_node.hpp>
#include "managers/animation_manager.hpp"


namespace pixelforge::scene::nodes {

    using animation::Animation;
    using transform::Transform;
    using std::string;
    using std::shared_ptr;


    AnimationNode::AnimationNode(const string& name):
        TransformNode(name) {
    }


    void AnimationNode::addAnimation(const shared_ptr<Animation>& animation) {
        if (animation) {
            animation->reset();
            animations_.push_back(management::AnimationManager::getOrCreate(animation));
            markGlobalTransformDirty();
        }
    }


    void AnimationNode::removeAnimation(const shared_ptr<Animation>& animation) {
        erase(animations_, animation);
        markGlobalTransformDirty();
    }


    void AnimationNode::playAllAnimations() const {
        forEachAnimation(&Animation::play);
    }


    void AnimationNode::pauseAllAnimations() const {
        forEachAnimation(&Animation::pause);
    }


    void AnimationNode::resumeAllAnimations() const {
        forEachAnimation(&Animation::resume);
    }


    void AnimationNode::stopAllAnimations() const {
        forEachAnimation(&Animation::stop);
    }


    void AnimationNode::resetAllAnimations() const {
        forEachAnimation(&Animation::reset);
    }


    void AnimationNode::updateAllAnimations(const float timeDelta) {
        for (const auto& animation : animations_) {
            animation->update(timeDelta);
        }
        markGlobalTransformDirty();
    }


    Transform AnimationNode::getLocalTransform() const {
        Transform combinedAnimationOffset;
        for (const auto& animation : animations_) {
            combinedAnimationOffset = combinedAnimationOffset * animation->getOffset();
        }
        return localTransform_ * combinedAnimationOffset;
    }


    void AnimationNode::updateSelf(const float timeDelta) {
        updateAllAnimations(timeDelta);
        if (globalTransformDirty_) {
            updateGlobalTransform();
        }
    }


    void AnimationNode::forEachAnimation(void (Animation::*action)()) const {
        for (const auto& animation : animations_) {
            (animation.get()->*action)();
        }
    }

} // namespace pixelforge::scene::nodes
