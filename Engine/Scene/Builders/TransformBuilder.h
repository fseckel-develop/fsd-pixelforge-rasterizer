#pragma once
#include "../Transforms/Transform.h"


class TransformBuilder {
public:
    TransformBuilder() = default;

    auto& fromMatrix(const mat4& matrix) {
        transform.SetFromMatrix(matrix);
        return *this;
    }

    auto& withTranslation(const vec3& translation) {
        transform.SetTranslation(translation);
        return *this;
    }

    auto& withTranslation(const float translateX, const float translateY, const float translateZ) {
        transform.SetTranslation(translateX, translateY, translateZ);
        return *this;
    }

    auto& withRotation(const quat& rotation) {
        transform.SetRotation(rotation);
        return *this;
    }

    auto& withRotation(const vec3& eulerAngles) {
        transform.SetRotation(eulerAngles);
        return *this;
    }

    auto& withRotation(const float pitch, const float yaw, const float roll) {
        transform.SetRotation(pitch, yaw, roll);
        return *this;
    }

    auto& withScale(const vec3& scale) {
        transform.SetScale(scale);
        return *this;
    }

    auto& withScale(const float scale) {
        transform.SetScale(scale);
        return *this;
    }

    auto& withScale(const float scaleX, const float scaleY, const float scaleZ) {
        transform.SetScale(scaleX, scaleY, scaleZ);
        return *this;
    }

    Transform Build() const {
        return transform;
    }

    operator Transform() { // NOLINT
        return Build();
    }

private:
    Transform transform;
};


inline auto Transform_() {
    return TransformBuilder();
}
