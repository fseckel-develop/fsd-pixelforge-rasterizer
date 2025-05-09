#include "Transform.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
using namespace glm;


Transform::Transform():
    translation(vec3(0.0f)),
    rotation(quat(1.0f, 0.0f, 0.0f, 0.0f)),
    scale(vec3(1.0f)) {
}


Transform::Transform(const vec3& translation, const quat& rotation, const vec3& scaling):
    translation(translation),
    rotation(rotation),
    scale(scaling) {
}


Transform::Transform(const vec3& translation, const vec3& eulerAngles, const vec3& scaling):
    translation(translation),
    rotation(quat(eulerAngles)),
    scale(scaling) {
}


Transform::Transform(const mat4& matrix):
    translation(vec3(0.0f)),
    rotation(quat(1.0f, 0.0f, 0.0f, 0.0f)),
    scale(vec3(1.0f)) {
    ComposeFromMatrix(matrix);
}


void Transform::ComposeFromMatrix(const mat4& matrix) {
    translation = vec3(matrix[3]);
    vec3 scaling;
    scaling.x = length(vec3(matrix[0]));
    scaling.y = length(vec3(matrix[1]));
    scaling.z = length(vec3(matrix[2]));
    scale = scaling;
    auto rotationMatrix = mat3(matrix);
    rotationMatrix[0] /= scaling.x;
    rotationMatrix[1] /= scaling.y;
    rotationMatrix[2] /= scaling.z;
    rotation = quat_cast(rotationMatrix);
}



void Transform::SetTranslation(const vec3& translation) {
    this->translation = translation;
}


void Transform::SetTranslation(const float translateX, const float translateY, const float translateZ) {
    SetTranslation(vec3(translateX, translateY, translateZ));
}


void Transform::SetRotation(const quat& rotation) {
    this->rotation = rotation;
}


void Transform::SetRotation(const vec3& eulerAngles) {
    SetRotation(quat(radians(eulerAngles)));
}


void Transform::SetRotation(const float rotationX, const float rotationY, const float rotationZ) {
    SetRotation(vec3(rotationX, rotationY, rotationZ));
}


void Transform::SetScale(const vec3& scale) {
    this->scale = scale;
}


void Transform::SetScale(const float scale) {
    SetScale(vec3(scale));
}


void Transform::SetScale(const float scaleX, const float scaleY, const float scaleZ) {
    SetScale(vec3(scaleX, scaleY, scaleZ));
}


mat4 Transform::ToMatrix() const {
    mat4 matrix = translate(mat4(1.0f), translation);
    matrix *= mat4_cast(rotation);
    matrix = glm::scale(matrix, scale);
    return matrix;
}


const vec3& Transform::GetTranslation() const {
    return this->translation;
}


const quat& Transform::GetRotation() const {
    return this->rotation;
}


const vec3& Transform::GetScale() const {
    return this->scale;
}


Transform Transform::operator*(const Transform& other) const {
    Transform result;
    result.translation = translation + other.translation;
    result.rotation = rotation * other.rotation;
    result.scale = scale * other.scale;
    return result;
}
