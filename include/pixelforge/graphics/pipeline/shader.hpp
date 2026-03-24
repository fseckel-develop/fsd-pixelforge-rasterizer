#pragma once
#include <GL/glew.h>
#include <GLM/gtc/type_ptr.hpp>
#include <unordered_set>

namespace pixelforge::graphics { class Material; }
namespace pixelforge::scene::lighting { class Light; }

namespace pixelforge::graphics {

    template<class> inline constexpr bool always_false_v = false;

    /// Encapsulates the creation, linking, and usage of an OpenGL shader.
    class Shader {
    public:
        /// Constructs a new Shader object and creates an OpenGL handle.
        Shader();

        /// Adds and compiles a shader module from a source file and attaches it to the shader.
        /// @param moduleType The type of shader module (e.g. GL_VERTEX_SHADER).
        /// @param moduleFileName Path of the shader module file relative to the shader directory.
        void addModule(GLenum moduleType, const char* moduleFileName) const;

        /// Links and validates the shader after all shader modules have been attached.
        void activateShader() const;

        /// Template specialization for setting uniform values.
        /// Supports int, bool, float, vec3, vec4, mat4.
        /// @tparam ValueT The type of the value to upload.
        /// @param name The uniform name in the shader.
        /// @param value The value to send to the GPU.
        template<typename ValueT>
        void setUniform(const char* name, const ValueT& value) {
            bindShader();
            if (const GLint location = getUniformLocation(name); location != -1) {
                if constexpr (std::is_same_v<ValueT, int> || std::is_same_v<ValueT, bool>) {
                    glUniform1i(location, value);
                } else if constexpr (std::is_same_v<ValueT, float>) {
                    glUniform1f(location, value);
                } else if constexpr (std::is_same_v<ValueT, glm::vec3>) {
                    glUniform3fv(location, 1, glm::value_ptr(value));
                } else if constexpr (std::is_same_v<ValueT, glm::vec4>) {
                    glUniform4fv(location, 1, glm::value_ptr(value));
                } else if constexpr (std::is_same_v<ValueT, glm::mat4>) {
                    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
                } else {
                    static_assert(always_false_v<ValueT>, "Unsupported type in Shader::setUniform");
                }
            }
            unbindShader();
        }

        /// Binds the shader to the current OpenGL context.
        void bindShader() const;

        /// Unbinds the currently active shader from the OpenGL context.
        static void unbindShader();

        /// Deletes the shader and frees its OpenGL resources.
        void deleteShader();

        /// Destructor. Ensures the shader is deleted.
        ~Shader();

    private:
        GLuint shaderID_; ///< OpenGL ID for the shader.
        std::unordered_map<std::string, GLint> uniformLocations_; ///< Cache of uniform names to their locations.

        /// Links the attached shader modules into a final shader.
        void linkShaderModules() const;

        /// Validates the shader in the current OpenGL context.
        void validateShader() const;

        /// Retrieves the location of a uniform variable, caching it for future lookups.
        /// @param name The name of the uniform variable.
        /// @return The OpenGL location ID of the uniform, or -1 if not found.
        GLint getUniformLocation(const char* name);

        /// Reads the full contents of a shader module file.
        /// @param filePath The file path to read.
        /// @return The shader module code as a string.
        static std::string readShaderModuleFile(const char* filePath);

        /// Recursively parses and expands '#include' directives in shader module code.
        /// @param shaderModule The shader module code to process.
        static void parseIncludes(std::string& shaderModule);
    };

} // namespace pixelforge::graphics
