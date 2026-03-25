#pragma once
#include <GL/glew.h>


namespace pixelforge::graphics::debug {

    /// Prints the shader info log for a given shader module.
    /// @param shaderID The OpenGL shader ID.
    void printShaderInfoLog(GLuint shaderID);

    /// Prints the program info log for a given shader program.
    /// @param programID The OpenGL program ID.
    void printProgramInfoLog(GLuint programID);

    /// Returns a string representation of the shader type.
    /// @param shaderType The OpenGL shader type.
    /// @return A string representing the shader type.
    const char* getShaderTypeName(GLenum shaderType);

    /// Checks for OpenGL errors and prints any errors encountered.
    /// @return True if errors were found, false otherwise.
    bool checkOpenGLError();

} // namespace pixelforge::graphics::debug
