#include "Transform.h"
#include <GLM/gtc/quaternion.hpp>


Transform::Transform() = default;


Transform::Transform(const vec3& translation, const quat& rotation, const vec3& scaling) {
    this->translation = translation;
    this->rotation = rotation;
    this->scaling = scaling;
}


Transform::Transform(const vec3& translation, const vec3& eulerAngles, const vec3& scaling) {
    this->translation = translation;
    this->rotation = quat(eulerAngles);
    this->scaling = scaling;
}


Transform::Transform(const mat4& matrix) {
    this->translation = vec3(matrix[3]);
    vec3 scaling;
    scaling.x = length(vec3(matrix[0]));
    scaling.y = length(vec3(matrix[1]));
    scaling.z = length(vec3(matrix[2]));
    this->scaling = scaling;
    auto rotationMatrix = mat3(matrix);
    rotationMatrix[0] /= scaling.x;
    rotationMatrix[1] /= scaling.y;
    rotationMatrix[2] /= scaling.z;
    this->rotation = quat_cast(rotationMatrix);
}


Transform Transform::operator*(const Transform& other) const {
    Transform result;
    result.translation = translation + other.translation;
    result.rotation = rotation * other.rotation;
    result.scaling = scaling * other.scaling;
    return result;
}


mat4 Transform::ToMatrix() const {
    mat4 matrix = translate(mat4(1.0f), translation);
    matrix *= mat4_cast(rotation);
    matrix = scale(matrix, scaling);
    return matrix;
}
