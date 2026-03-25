#include "graphics/debug.hpp"
#include <iostream>
#include <string>


namespace pixelforge::graphics::debug {

    void printShaderInfoLog(const GLuint shaderID) {
        int length = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            std::string log(static_cast<size_t>(length), '\0');
            glGetShaderInfoLog(shaderID, length, &length, log.data());
            std::cout << "\tShader Info Log:\n" << log << std::endl;
        }
    }

    void printProgramInfoLog(const GLuint programID) {
        int length = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            std::string log(static_cast<size_t>(length), '\0');
            glGetProgramInfoLog(programID, length, &length, log.data());
            std::cout << "\tProgram Info Log:\n" << log << std::endl;
        }
    }

    const char* getShaderTypeName(const GLenum shaderType) {
        switch (shaderType) {
            case GL_VERTEX_SHADER: return "vertex shader";
            case GL_FRAGMENT_SHADER: return "fragment shader";
            case GL_GEOMETRY_SHADER: return "geometry shader";
            case GL_COMPUTE_SHADER: return "compute shader";
            case GL_TESS_CONTROL_SHADER: return "tess control shader";
            case GL_TESS_EVALUATION_SHADER: return "tess evaluation shader";
            default: return "unknown shader";
        }
    }

    bool checkOpenGLError() {
        bool found = false;
        GLenum error = glGetError();
        while (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << error << std::endl;
            found = true;
            error = glGetError();
        }
        return found;
    }

} // namespace pixelforge::graphics::debug
