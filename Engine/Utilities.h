#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLM/fwd.hpp>
#include "GLFW/glfw3.h"
#include <iostream>
#include <GLM/detail/type_quat.hpp>
#include <GLM/ext/matrix_transform.hpp>
using namespace glm; using namespace std;


class Utilities {
public:
    static const char* PrintShaderType(GLenum);
    static void PrintShaderInfoLog(GLuint);
    static void PrintProgramInfoLog(GLuint);
    static bool CheckOpenGLError();
    static void InitializeGLFW();
    static void InitializeGLEW();

    static const char* UniformArrayName(const char*, int, const char*);

    static vector<vec3> ScaledCopy(const vector<vec3>&, vec3);
    static vector<vec3> TranslatedCopy(const vector<vec3>&, vec3);
    static vector<vec3> RotatedCopy(const vector<vec3>&, float, vec3);
    static vector<vec3> RotatedCopy(const vector<vec3>&, quat);
    static void Scale(vector<vec3>&, vec3);
    static void Translate(vector<vec3>&, vec3);
    static void Rotate(vector<vec3>&, float, vec3);
    static void Rotate(vector<vec3>&, quat);
};


inline void Utilities::PrintShaderInfoLog(const GLuint shaderID) {
    int infoLogLength;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        const auto infoLog = new char[infoLogLength];
        glGetShaderInfoLog(shaderID, infoLogLength, &infoLogLength, infoLog);
        cout << "\tShader Info Log: " << endl << infoLog << endl;
        delete[] infoLog;
    }
}


inline void Utilities::PrintProgramInfoLog(const GLuint programID) {
    int infoLogLength;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        const auto infoLog = new char[infoLogLength];
        glGetProgramInfoLog(programID, infoLogLength, &infoLogLength, infoLog);
        cout << "\tProgram Info Log: " << endl << infoLog << endl;
        delete[] infoLog;
    }
}


inline const char* Utilities::PrintShaderType(const GLenum shaderType) {
    switch (shaderType) {
        case GL_VERTEX_SHADER: return "Vertex Shader";
        case GL_FRAGMENT_SHADER: return "Fragment Shader";
        case GL_GEOMETRY_SHADER: return "Geometry Shader";
        case GL_COMPUTE_SHADER: return "Compute Shader";
        case GL_TESS_CONTROL_SHADER: return "Tesselation Control Shader";
        case GL_TESS_EVALUATION_SHADER: return "Tesselation Evaluation Shader";
        default: return "Shader of Unknown Type";
    }
}


inline bool Utilities::CheckOpenGLError() {
    bool foundError = false;
    GLuint glError = glGetError();
    while (glError != GL_NO_ERROR) {
        cerr << "OpenGL Error: " << glError << endl;
        foundError = true;
        glError = glGetError();
    }
    return foundError;
}


inline void Utilities::InitializeGLFW() {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW." << endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}


inline void Utilities::InitializeGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW." << endl;
    }
}


inline const char* ArrayElement(const char* base, const int index, const char* attribute) {
    static char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s[%d].%s", base, index, attribute);
    return buffer;
}


inline vector<vec3> Utilities::ScaledCopy(const vector<vec3>& vertices, const vec3 scale) {
    vector<vec3> newVertices = vertices;
    Scale(newVertices, scale);
    return newVertices;
}


inline vector<vec3> Utilities::TranslatedCopy(const vector<vec3>& vertices, const vec3 translate) {
    vector<vec3> newVertices = vertices;
    Translate(newVertices, translate);
    return newVertices;
}


inline vector<vec3> Utilities::RotatedCopy(const vector<vec3>& vertices, const float angle, const vec3 axis) {
    vector<vec3> newVertices = vertices;
    Rotate(newVertices, angle, axis);
    return newVertices;
}


inline vector<vec3> Utilities::RotatedCopy(const vector<vec3>& vertices, const quat rotation) {
    vector<vec3> newVertices = vertices;
    Rotate(newVertices, rotation);
    return newVertices;
}


inline void Utilities::Scale(vector<vec3>& vertices, const vec3 scale) {
    for (auto& vertex : vertices) {
        vertex *= scale;
    }
}


inline void Utilities::Translate(vector<vec3>& vertices, const vec3 translate) {
    for (auto& vertex : vertices) {
        vertex += translate;
    }
}


inline void Utilities::Rotate(vector<vec3>& vertices, const float angle, const vec3 axis) {
    const mat4 rotation = rotate(mat4(1.0f), angle, axis);
    for (auto& vertex : vertices) {
        vec4 temp = rotation * vec4(vertex, 1.0f);
        vertex = vec3(temp);
    }
}


inline void Utilities::Rotate(vector<vec3>& vertices, const quat rotation) {
    for (auto& vertex : vertices) {
        vertex = rotation * vertex;
    }
}
