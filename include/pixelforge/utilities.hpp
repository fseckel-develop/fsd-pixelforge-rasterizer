#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <iostream>


namespace pixelforge {

    /// Various helper functions for OpenGL set-up and debugging as well as vector manipulation.
    class Utilities {
    public:
        /// Initializes the GLFW library and sets OpenGL context version and profile.
        static void initializeGLFW() {
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW." << std::endl;
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        }

        /// Initializes the GLEW library.
        static void initializeGLEW() {
            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) {
                std::cerr << "Failed to initialize GLEW." << std::endl;
            }
        }

        /// Prints the shader info log for a given shader module.
        /// @param shaderID The OpenGL shader ID.
        static void printShaderInfoLog(const GLuint shaderID) {
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
        static void printProgramInfoLog(const GLuint programID) {
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
        static const char* printShaderType(const GLenum shaderType) {
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
        static bool checkOpenGLError() {
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
        static const char* getUniformArray(const char* uniform, const size_t index, const char* attribute = nullptr) {
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
        /// @param translateBy The translation vector to apply.
        /// @return A new vector of translated vertices.
        static std::vector<glm::vec3> getTranslatedCopy(const std::vector<glm::vec3>& vertices, const glm::vec3 translateBy) {
            std::vector<glm::vec3> newVertices = vertices;
            translate(newVertices, translateBy);
            return newVertices;
        }

        /// Creates a rotated copy of a vector of vertices based on an angle and axis.
        /// @param vertices The original vertices.
        /// @param angle The rotation angle in radians.
        /// @param axis The axis around which to rotate.
        /// @return A new vector of rotated vertices.
        static std::vector<glm::vec3> getRotatedCopy(const std::vector<glm::vec3>& vertices, const float angle, const glm::vec3 axis) {
            std::vector<glm::vec3> newVertices = vertices;
            rotate(newVertices, angle, axis);
            return newVertices;
        }

        /// Creates a rotated copy of a vector of vertices based on a quaternion rotation.
        /// @param vertices The original vertices.
        /// @param rotation The quaternion representing the rotation.
        /// @return A new vector of rotated vertices.
        static std::vector<glm::vec3> getRotatedCopy(const std::vector<glm::vec3>& vertices, const glm::quat rotation) {
            std::vector<glm::vec3> newVertices = vertices;
            rotate(newVertices, rotation);
            return newVertices;
        }

        /// Creates a scaled copy of a vector of vertices.
        /// @param vertices The original vertices.
        /// @param scaleBy The scale factor to apply.
        /// @return A new vector of scaled vertices.
        static std::vector<glm::vec3> getScaledCopy(const std::vector<glm::vec3>& vertices, const glm::vec3 scaleBy) {
            std::vector<glm::vec3> newVertices = vertices;
            scale(newVertices, scaleBy);
            return newVertices;
        }

        /// Translates a vector of vertices by a given translation vector.
        /// @param vertices The vertices to translate.
        /// @param translateBy The translation vector.
        static void translate(std::vector<glm::vec3>& vertices, const glm::vec3 translateBy) {
            for (auto& vertex : vertices) {
                vertex += translateBy;
            }
        }

        /// Rotates a vector of vertices by a given angle and axis.
        /// @param vertices The vertices to rotate.
        /// @param angle The rotation angle in radians.
        /// @param axis The axis around which to rotate.
        static void rotate(std::vector<glm::vec3>& vertices, const float angle, const glm::vec3 axis) {
            const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
            for (auto& vertex : vertices) {
                glm::vec4 temp = rotation * glm::vec4(vertex, 1.0f);
                vertex = glm::vec3(temp);
            }
        }

        /// Rotates a vector of vertices by a given quaternion rotation.
        /// @param vertices The vertices to rotate.
        /// @param rotateBy The quaternion representing the rotation.
        static void rotate(std::vector<glm::vec3>& vertices, const glm::quat rotateBy) {
            for (auto& vertex : vertices) {
                vertex = rotateBy * vertex;
            }
        }

        /// Scales a vector of vertices by a given scale factor.
        /// @param vertices The vertices to scale.
        /// @param scaleBy The scale factor.
        static void scale(std::vector<glm::vec3>& vertices, const glm::vec3 scaleBy) {
            for (auto& vertex : vertices) {
                vertex *= scaleBy;
            }
        }

        /// Validates and normalizes a direction vector.
        /// @param direction The direction vector to validate.
        /// @param context A string representing the context of the direction.
        /// @return The normalized direction vector.
        /// @throws std::invalid_argument If the direction vector is zero-length.
        static glm::vec3 validateDirection(const glm::vec3& direction, const std::string& context) {
            if (glm::length(direction) < 1e-6f) {
                throw std::invalid_argument("Invalid zero direction vector" + (context.empty() ? "" : " in " + context));
            }
            return glm::normalize(direction);
        }
    };

} // namespace pixelforge;
