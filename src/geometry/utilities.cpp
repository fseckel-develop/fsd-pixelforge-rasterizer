#include <pixelforge/geometry/utilities.hpp>


namespace pixelforge::geometry::utilities {

    void translate(std::vector<glm::vec3>& vertices, const glm::vec3 translateBy) {
        for (auto& vertex : vertices) {
            vertex += translateBy;
        }
    }

    void rotate(std::vector<glm::vec3>& vertices, const float angle, const glm::vec3 axis) {
        const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
        for (auto& vertex : vertices) {
            glm::vec4 temp = rotation * glm::vec4(vertex, 1.0f);
            vertex = glm::vec3(temp);
        }
    }

    void rotate(std::vector<glm::vec3>& vertices, const glm::quat rotateBy) {
        for (auto& vertex : vertices) {
            vertex = rotateBy * vertex;
        }
    }

    void scale(std::vector<glm::vec3>& vertices, const glm::vec3 scaleBy) {
        for (auto& vertex : vertices) {
            vertex *= scaleBy;
        }
    }

    std::vector<glm::vec3> getTranslatedCopy(const std::vector<glm::vec3>& vertices, const glm::vec3 translateBy) {
        std::vector<glm::vec3> newVertices = vertices;
        translate(newVertices, translateBy);
        return newVertices;
    }

    std::vector<glm::vec3> getRotatedCopy(const std::vector<glm::vec3>& vertices, const float angle, const glm::vec3 axis) {
        std::vector<glm::vec3> newVertices = vertices;
        rotate(newVertices, angle, axis);
        return newVertices;
    }

    std::vector<glm::vec3> getRotatedCopy(const std::vector<glm::vec3>& vertices, const glm::quat rotation) {
        std::vector<glm::vec3> newVertices = vertices;
        rotate(newVertices, rotation);
        return newVertices;
    }

    std::vector<glm::vec3> getScaledCopy(const std::vector<glm::vec3>& vertices, const glm::vec3 scaleBy) {
        std::vector<glm::vec3> newVertices = vertices;
        scale(newVertices, scaleBy);
        return newVertices;
    }

} // namespace