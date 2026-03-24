#include <pixelforge/scene/transform/transform.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>


namespace pixelforge::scene::transform {

    using glm::vec3;
    using glm::vec4;
    using glm::mat3;
    using glm::mat4;
    using glm::quat;


    Transform::Transform():
        translation_(vec3(0.0f)),
        rotation_(quat(1.0f, 0.0f, 0.0f, 0.0f)),
        scale_(vec3(1.0f)) {
    }


    Transform::Transform(const vec3& translation, const quat& rotation, const vec3& scaling):
        translation_(translation),
        rotation_(rotation),
        scale_(scaling) {
    }


    Transform::Transform(const vec3& translation, const vec3& eulerAngles, const vec3& scaling):
        translation_(translation),
        rotation_(quat(eulerAngles)),
        scale_(scaling) {
    }


    Transform::Transform(const mat4& matrix):
        translation_(vec3(0.0f)),
        rotation_(quat(1.0f, 0.0f, 0.0f, 0.0f)),
        scale_(vec3(1.0f)) {
        composeFromMatrix(matrix);
    }


    void Transform::composeFromMatrix(const mat4& matrix) {
        translation_ = vec3(matrix[3]);
        vec3 scaling;
        scaling.x = length(vec3(matrix[0]));
        scaling.y = length(vec3(matrix[1]));
        scaling.z = length(vec3(matrix[2]));
        scale_ = scaling;
        auto rotationMatrix = mat3(matrix);
        rotationMatrix[0] /= scaling.x;
        rotationMatrix[1] /= scaling.y;
        rotationMatrix[2] /= scaling.z;
        rotation_ = quat_cast(rotationMatrix);
    }



    void Transform::setTranslation(const vec3& translation) {
        this->translation_ = translation;
    }


    void Transform::setTranslation(const float translateX, const float translateY, const float translateZ) {
        setTranslation(vec3(translateX, translateY, translateZ));
    }


    void Transform::setRotation(const quat& rotation) {
        this->rotation_ = rotation;
    }


    void Transform::setRotation(const vec3& eulerAngles) {
        setRotation(quat(radians(eulerAngles)));
    }


    void Transform::setRotation(const float rotationX, const float rotationY, const float rotationZ) {
        setRotation(vec3(rotationX, rotationY, rotationZ));
    }


    void Transform::setScale(const vec3& scale) {
        this->scale_ = scale;
    }


    void Transform::setScale(const float scale) {
        setScale(vec3(scale));
    }


    void Transform::setScale(const float scaleX, const float scaleY, const float scaleZ) {
        setScale(vec3(scaleX, scaleY, scaleZ));
    }


    mat4 Transform::toMatrix() const {
        mat4 matrix = translate(mat4(1.0f), translation_);
        matrix *= mat4_cast(rotation_);
        matrix = scale(matrix, scale_);
        return matrix;
    }


    const vec3& Transform::getTranslation() const {
        return this->translation_;
    }


    const quat& Transform::getRotation() const {
        return this->rotation_;
    }


    const vec3& Transform::getScale() const {
        return this->scale_;
    }


    Transform Transform::operator*(const Transform& other) const {
        Transform result;
        result.translation_ = translation_ + other.translation_;
        result.rotation_ = rotation_ * other.rotation_;
        result.scale_ = scale_ * other.scale_;
        return result;
    }

} // namespace pixelforge::scene::transform
