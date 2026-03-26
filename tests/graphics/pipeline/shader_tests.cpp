#include <doctest.h>
#include <graphics/pipeline/shader.hpp>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using pixelforge::graphics::Shader;


namespace {

    namespace fs = std::filesystem;

    fs::path shaderDir() {
        return fs::path(PIXELFORGE_ASSET_DIR) / "shaders";
    }

    struct TempShaderFiles {
        std::vector<fs::path> files;

        ~TempShaderFiles() {
            for (const auto& file : files) {
                std::error_code ec;
                fs::remove(file, ec);
            }
        }

        void write(const std::string& fileName, const std::string& content) {
            const fs::path fullPath = shaderDir() / fileName;
            std::ofstream out(fullPath);
            REQUIRE_MESSAGE(out.is_open(), "Failed to create temporary shader file: " << fullPath.string());
            out << content;
            out.close();
            files.push_back(fullPath);
        }
    };

    std::string uniqueName(const std::string& prefix, const std::string& extension) {
        static int counter = 0;
        return prefix + "_" + std::to_string(counter++) + extension;
    }

    TempShaderFiles makeMinimalShaderPair(std::string& vertexName, std::string& fragmentName) {
        TempShaderFiles tempFiles;

        vertexName = uniqueName("test_shader_vertex", ".vert");
        fragmentName = uniqueName("test_shader_fragment", ".frag");

        tempFiles.write(vertexName,
            "#version 410 core\n"
            "layout(location = 0) in vec3 aPosition;\n"
            "uniform mat4 uTransform;\n"
            "void main() {\n"
            "    gl_Position = uTransform * vec4(aPosition, 1.0);\n"
            "}\n");

        tempFiles.write(fragmentName,
            "#version 410 core\n"
            "out vec4 FragColor;\n"
            "uniform vec4 uColor;\n"
            "void main() {\n"
            "    FragColor = uColor;\n"
            "}\n");

        return tempFiles;
    }

    TempShaderFiles makeShaderPairWithInclude(std::string& vertexName, std::string& fragmentName, std::string& includeName) {
        TempShaderFiles tempFiles;

        includeName = uniqueName("test_shader_include", ".glsl");
        vertexName = uniqueName("test_shader_inc_vertex", ".vert");
        fragmentName = uniqueName("test_shader_inc_fragment", ".frag");

        tempFiles.write(includeName,
            "vec4 makeColor() {\n"
            "    return vec4(0.25, 0.5, 0.75, 1.0);\n"
            "}\n");

        tempFiles.write(vertexName,
            "#version 410 core\n"
            "layout(location = 0) in vec3 aPosition;\n"
            "void main() {\n"
            "    gl_Position = vec4(aPosition, 1.0);\n"
            "}\n");

        tempFiles.write(fragmentName,
            "#version 410 core\n"
            "out vec4 FragColor;\n"
            "#include \"" + includeName + "\"\n"
            "void main() {\n"
            "    FragColor = makeColor();\n"
            "}\n");

        return tempFiles;
    }

} // namespace


TEST_CASE("Shader constructs a valid OpenGL program object") {
    glGetError();

    Shader shader;

    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Shader can add modules and activate program") {
    std::string vertexName;
    std::string fragmentName;
    const auto tempFiles = makeMinimalShaderPair(vertexName, fragmentName);

    glGetError();

    const Shader shader;
    shader.addModule(GL_VERTEX_SHADER, vertexName.c_str());
    shader.addModule(GL_FRAGMENT_SHADER, fragmentName.c_str());
    shader.activateShader();

    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Shader bindShader binds a linked shader program") {
    std::string vertexName;
    std::string fragmentName;
    const auto tempFiles = makeMinimalShaderPair(vertexName, fragmentName);

    Shader shader;
    shader.addModule(GL_VERTEX_SHADER, vertexName.c_str());
    shader.addModule(GL_FRAGMENT_SHADER, fragmentName.c_str());
    shader.activateShader();

    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    CHECK(currentProgram == 0);

    shader.bindShader();

    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    CHECK(currentProgram != 0);
}

TEST_CASE("Shader unbindShader clears the currently bound program") {
    std::string vertexName;
    std::string fragmentName;
    const auto tempFiles = makeMinimalShaderPair(vertexName, fragmentName);

    Shader shader;
    shader.addModule(GL_VERTEX_SHADER, vertexName.c_str());
    shader.addModule(GL_FRAGMENT_SHADER, fragmentName.c_str());
    shader.activateShader();
    shader.bindShader();

    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    REQUIRE(currentProgram != 0);

    Shader::unbindShader();

    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    CHECK(currentProgram == 0);
}

TEST_CASE("Shader setUniform uploads supported uniform types without OpenGL error") {
    std::string vertexName;
    std::string fragmentName;
    const auto tempFiles = makeMinimalShaderPair(vertexName, fragmentName);

    Shader shader;
    shader.addModule(GL_VERTEX_SHADER, vertexName.c_str());
    shader.addModule(GL_FRAGMENT_SHADER, fragmentName.c_str());
    shader.activateShader();

    glGetError();

    shader.setUniform("uTransform", glm::mat4(1.0f));
    CHECK(glGetError() == GL_NO_ERROR);

    shader.setUniform("uColor", glm::vec4(1.0f, 0.5f, 0.25f, 1.0f));
    CHECK(glGetError() == GL_NO_ERROR);

    shader.setUniform("uMissingFloat", 1.0f);
    CHECK(glGetError() == GL_NO_ERROR);

    shader.setUniform("uMissingVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    CHECK(glGetError() == GL_NO_ERROR);

    shader.setUniform("uMissingInt", 3);
    CHECK(glGetError() == GL_NO_ERROR);

    shader.setUniform("uMissingBool", true);
    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Shader parseIncludes supports fragment shader includes") {
    std::string vertexName;
    std::string fragmentName;
    std::string includeName;
    const auto tempFiles = makeShaderPairWithInclude(vertexName, fragmentName, includeName);

    glGetError();

    Shader shader;
    shader.addModule(GL_VERTEX_SHADER, vertexName.c_str());
    shader.addModule(GL_FRAGMENT_SHADER, fragmentName.c_str());
    shader.activateShader();

    CHECK(glGetError() == GL_NO_ERROR);
}

TEST_CASE("Shader deleteShader unbinds current program without OpenGL error") {
    std::string vertexName;
    std::string fragmentName;
    const auto tempFiles = makeMinimalShaderPair(vertexName, fragmentName);

    Shader shader;
    shader.addModule(GL_VERTEX_SHADER, vertexName.c_str());
    shader.addModule(GL_FRAGMENT_SHADER, fragmentName.c_str());
    shader.activateShader();
    shader.bindShader();

    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    REQUIRE(currentProgram != 0);

    glGetError();
    shader.deleteShader();

    CHECK(glGetError() == GL_NO_ERROR);

    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    CHECK(currentProgram == 0);
}
