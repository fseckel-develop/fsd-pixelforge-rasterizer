#pragma once
#include <GLM/glm.hpp>
#include <GLM/detail/type_quat.hpp>
using namespace glm;


class Transform {
public:
    vec3 translation = vec3(0.0f);
    quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
    vec3 scaling = vec3(1.0f);

    Transform();
    explicit Transform(const mat4&);
    Transform(const vec3&, const quat&, const vec3&);
    Transform(const vec3&, const vec3&, const vec3&);
    static Transform FromMatrix(const mat4&);
    Transform operator*(const Transform& other) const;
    [[nodiscard]] mat4 ToMatrix() const;
};
