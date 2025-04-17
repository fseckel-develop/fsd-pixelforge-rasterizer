#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <unordered_map>
#include <unordered_set>
class Material; class Light;
using namespace std; using namespace glm;


class ShaderProgram {
public:
    ShaderProgram();
    void AddShader(GLenum, const char*) const;
    void ActivateProgram() const;
    template<typename T> void SetUniform(const char* name, const T& value);
    void UseProgram() const;
    static void UnuseProgram();
    void DeleteProgram();
    ~ShaderProgram();

private:
    GLuint programID;
    unordered_map<string, GLint> uniformLocations;
    void LinkProgram() const;
    void ValidateProgram() const;
    GLint GetUniformLocation(const char*);
    static string ReadShaderFile(const char*);
    static void ParseIncludes(string&);
};


template<typename T> inline constexpr bool alwaysFalse = false;
template<typename T> void ShaderProgram::SetUniform(const char* name, const T& value) {
    UseProgram();
    const GLint location = GetUniformLocation(name);
    if (location != -1) {
        if constexpr (is_same_v<T, int> || is_same_v<T, bool>) {
            glUniform1i(location, value);
        } else if constexpr (is_same_v<T, float>) {
            glUniform1f(location, value);
        } else if constexpr (is_same_v<T, vec3>) {
            glUniform3fv(location, 1, value_ptr(value));
        } else if constexpr (is_same_v<T, vec4>) {
            glUniform4fv(location, 1, value_ptr(value));
        } else if constexpr (is_same_v<T, mat4>) {
            glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
        } else {
            static_assert(alwaysFalse<T>, "Unsupported uniform type");
        }
    }
    UnuseProgram();
}
