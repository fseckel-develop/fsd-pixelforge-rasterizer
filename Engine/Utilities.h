#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <iostream>


/// Various helper functions for OpenGL set-up and debugging as well as vector manipulation.
class Utilities {
public:
    /// Initializes the GLFW library and sets OpenGL context version and profile.
    static void InitializeGLFW() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW." << std::endl;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    /// Initializes the GLEW library.
    static void InitializeGLEW() {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW." << std::endl;
        }
    }

    /// Prints the shader info log for a given shader module.
    /// @param shaderID The OpenGL shader ID.
    static void PrintShaderInfoLog(const GLuint shaderID) {
        int infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            const auto infoLog = new char[infoLogLength];
            glGetShaderInfoLog(shaderID, infoLogLength, &infoLogLength, infoLog);
            std::cout << "\tShader Info Log: " << std::endl << infoLog << std::endl;
            delete[] infoLog;
        }
    }

    /// Prints the program info log for a given shader program.
    /// @param programID The OpenGL program ID.
    static void PrintProgramInfoLog(const GLuint programID) {
        int infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            const auto infoLog = new char[infoLogLength];
            glGetProgramInfoLog(programID, infoLogLength, &infoLogLength, infoLog);
            std::cout << "\tProgram Info Log: " << std::endl << infoLog << std::endl;
            delete[] infoLog;
        }
    }

    /// Returns a string representation of the shader type.
    /// @param shaderType The OpenGL shader type.
    /// @return A string representing the shader type.
    static const char* PrintShaderType(const GLenum shaderType) {
        switch (shaderType) {
            case GL_VERTEX_SHADER: return "vertex shader";
            case GL_FRAGMENT_SHADER: return "fragment shader";
            case GL_GEOMETRY_SHADER: return "geometry shader";
            case GL_COMPUTE_SHADER: return "compute shader";
            case GL_TESS_CONTROL_SHADER: return "tesselation control shader";
            case GL_TESS_EVALUATION_SHADER: return "tesselation evaluation shader";
            default: return "shader of unknown type";
        }
    }

    /// Checks for OpenGL errors and prints any errors encountered.
    /// @return True if errors were found, false otherwise.
    static bool CheckOpenGLError() {
        bool foundError = false;
        GLuint glError = glGetError();
        while (glError != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << glError << std::endl;
            foundError = true;
            glError = glGetError();
        }
        return foundError;
    }

    /// Generates a string representation of a uniform array element.
    /// @param uniform The name of the array uniform.
    /// @param index The index of the array element.
    /// @param attribute The attribute name (if any).
    /// @return A string representing the array element.
    static const char* UniformArray(const char* uniform, const size_t index, const char* attribute = nullptr) {
        static char buffer[1024];
        if (attribute == nullptr) {
            snprintf(buffer, sizeof(buffer), "%s[%lu]", uniform, index);
        } else {
            snprintf(buffer, sizeof(buffer), "%s[%lu].%s", uniform, index, attribute);
        }
        return buffer;
    }

    /// Creates a translated copy of a vector of vertices.
    /// @param vertices The original vertices.
    /// @param translate The translation vector to apply.
    /// @return A new vector of translated vertices.
    static std::vector<glm::vec3> TranslatedCopy(const std::vector<glm::vec3>& vertices, const glm::vec3 translate) {
        std::vector<glm::vec3> newVertices = vertices;
        Translate(newVertices, translate);
        return newVertices;
    }

    /// Creates a rotated copy of a vector of vertices based on an angle and axis.
    /// @param vertices The original vertices.
    /// @param angle The rotation angle in radians.
    /// @param axis The axis around which to rotate.
    /// @return A new vector of rotated vertices.
    static std::vector<glm::vec3> RotatedCopy(const std::vector<glm::vec3>& vertices, const float angle, const glm::vec3 axis) {
        std::vector<glm::vec3> newVertices = vertices;
        Rotate(newVertices, angle, axis);
        return newVertices;
    }

    /// Creates a rotated copy of a vector of vertices based on a quaternion rotation.
    /// @param vertices The original vertices.
    /// @param rotation The quaternion representing the rotation.
    /// @return A new vector of rotated vertices.
    static std::vector<glm::vec3> RotatedCopy(const std::vector<glm::vec3>& vertices, const glm::quat rotation) {
        std::vector<glm::vec3> newVertices = vertices;
        Rotate(newVertices, rotation);
        return newVertices;
    }

    /// Creates a scaled copy of a vector of vertices.
    /// @param vertices The original vertices.
    /// @param scale The scale factor to apply.
    /// @return A new vector of scaled vertices.
    static std::vector<glm::vec3> ScaledCopy(const std::vector<glm::vec3>& vertices, const glm::vec3 scale) {
        std::vector<glm::vec3> newVertices = vertices;
        Scale(newVertices, scale);
        return newVertices;
    }

    /// Translates a vector of vertices by a given translation vector.
    /// @param vertices The vertices to translate.
    /// @param translate The translation vector.
    static void Translate(std::vector<glm::vec3>& vertices, const glm::vec3 translate) {
        for (auto& vertex : vertices) {
            vertex += translate;
        }
    }

    /// Rotates a vector of vertices by a given angle and axis.
    /// @param vertices The vertices to rotate.
    /// @param angle The rotation angle in radians.
    /// @param axis The axis around which to rotate.
    static void Rotate(std::vector<glm::vec3>& vertices, const float angle, const glm::vec3 axis) {
        const glm::mat4 rotation = rotate(glm::mat4(1.0f), angle, axis);
        for (auto& vertex : vertices) {
            glm::vec4 temp = rotation * glm::vec4(vertex, 1.0f);
            vertex = glm::vec3(temp);
        }
    }

    /// Rotates a vector of vertices by a given quaternion rotation.
    /// @param vertices The vertices to rotate.
    /// @param rotation The quaternion representing the rotation.
    static void Rotate(std::vector<glm::vec3>& vertices, const glm::quat rotation) {
        for (auto& vertex : vertices) {
            vertex = rotation * vertex;
        }
    }

    /// Scales a vector of vertices by a given scale factor.
    /// @param vertices The vertices to scale.
    /// @param scale The scale factor.
    static void Scale(std::vector<glm::vec3>& vertices, const glm::vec3 scale) {
        for (auto& vertex : vertices) {
            vertex *= scale;
        }
    }

    /// Validates and normalizes a direction vector.
    /// @param direction The direction vector to validate.
    /// @param context A string representing the context of the direction.
    /// @return The normalized direction vector.
    /// @throws std::invalid_argument If the direction vector is zero-length.
    static glm::vec3 ValidateDirection(const glm::vec3& direction, const std::string& context) {
        if (glm::length(direction) < 1e-6f) {
            throw std::invalid_argument("Invalid zero direction vector" + (context.empty() ? "" : " in " + context));
        }
        return glm::normalize(direction);
    }
};
