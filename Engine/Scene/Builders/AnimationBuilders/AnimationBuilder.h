#pragma once
#include <memory>
#include "../../Animations/Animation.h"


template<typename T>
concept IsAnimationBuilder = requires(T t) {
    { t.Build() } -> std::convertible_to<std::shared_ptr<Animation>>;
};


template<typename AnimationT, typename DerivedBuilder> class AnimationBuilder {
public:
    explicit AnimationBuilder(const Animation::Mode mode) {
        animation = std::make_shared<AnimationT>(mode);
    }

    auto& withDuration(const float duration) {
        animation->SetDuration(duration);
        return static_cast<DerivedBuilder&>(*this);
    }

    std::shared_ptr<AnimationT> Build() const {
        return animation;
    }

    operator std::shared_ptr<AnimationT>() { // NOLINT
        return Build();
    }

protected:
    std::shared_ptr<AnimationT> animation;
};
