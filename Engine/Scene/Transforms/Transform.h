#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
using namespace glm;


class Transform {
public:
    Transform();
    explicit Transform(const mat4&);
    Transform(const vec3&, const quat&, const vec3&);
    Transform(const vec3&, const vec3&, const vec3&);
    void SetFromMatrix(const mat4&);
    void SetTranslation(const vec3&);
    void SetTranslation(float, float, float);
    void SetRotation(const quat&);
    void SetRotation(const vec3&);
    void SetRotation(float, float, float);
    void SetScale(const vec3&);
    void SetScale(float);
    void SetScale(float, float, float);
    [[nodiscard]] mat4 ToMatrix() const;
    [[nodiscard]] const vec3& GetTranslation() const;
    [[nodiscard]] const quat& GetRotation() const;
    [[nodiscard]] const vec3& GetScale() const;
    Transform operator*(const Transform& other) const;

private:
    vec3 translation;
    quat rotation;
    vec3 scale;
};
