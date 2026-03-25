#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <vector>


namespace pixelforge::geometry::utilities {

    /// Translates a vector of vertices by a given translation vector.
    /// @param vertices The vertices to translate.
    /// @param translateBy The translation vector.
    void translate(std::vector<glm::vec3>& vertices, glm::vec3 translateBy);

    /// Rotates a vector of vertices by a given angle and axis.
    /// @param vertices The vertices to rotate.
    /// @param angle The rotation angle in radians.
    /// @param axis The axis around which to rotate.
    void rotate(std::vector<glm::vec3>& vertices, float angle, glm::vec3 axis);

    /// Rotates a vector of vertices by a given quaternion rotation.
    /// @param vertices The vertices to rotate.
    /// @param rotateBy The quaternion representing the rotation.
    void rotate(std::vector<glm::vec3>& vertices, glm::quat rotateBy);

    /// Scales a vector of vertices by a given scale factor.
    /// @param vertices The vertices to scale.
    /// @param scaleBy The scale factor.
    void scale(std::vector<glm::vec3>& vertices, glm::vec3 scaleBy);

    /// Creates a translated copy of a vector of vertices.
    /// @param vertices The original vertices.
    /// @param translateBy The translation vector to apply.
    /// @return A new vector of translated vertices.
    std::vector<glm::vec3> getTranslatedCopy(const std::vector<glm::vec3>& vertices, glm::vec3 translateBy);

    /// Creates a rotated copy of a vector of vertices based on an angle and axis.
    /// @param vertices The original vertices.
    /// @param angle The rotation angle in radians.
    /// @param axis The axis around which to rotate.
    /// @return A new vector of rotated vertices.
    std::vector<glm::vec3> getRotatedCopy(const std::vector<glm::vec3>& vertices, float angle, glm::vec3 axis);

    /// Creates a rotated copy of a vector of vertices based on a quaternion rotation.
    /// @param vertices The original vertices.
    /// @param rotation The quaternion representing the rotation.
    /// @return A new vector of rotated vertices.
    std::vector<glm::vec3> getRotatedCopy(const std::vector<glm::vec3>& vertices, glm::quat rotation);

    /// Creates a scaled copy of a vector of vertices.
    /// @param vertices The original vertices.
    /// @param scaleBy The scale factor to apply.
    /// @return A new vector of scaled vertices.
    std::vector<glm::vec3> getScaledCopy(const std::vector<glm::vec3>& vertices, glm::vec3 scaleBy);

} // pixelforge::geometry::utilities
