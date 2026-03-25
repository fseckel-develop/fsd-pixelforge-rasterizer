#include <pixelforge/utilities.hpp>
#include <fstream>
#include <sstream>
#include "graphics/pipeline/shader.hpp"


namespace pixelforge::graphics {

    using std::vector;
    using std::unordered_set;
    using std::string;
    using std::ifstream;
    using std::ios;
    using std::istringstream;
    using std::stringstream;
    using std::istreambuf_iterator;
    using std::filesystem::path;
    using std::cerr;
    using std::endl;


    Shader::Shader() {
        shaderID_ = glCreateProgram();
        if (!shaderID_) {
            cerr << "Failed to create shader program." << endl;
        }
    }


    const path shaderDirectory = path(PIXELFORGE_ASSET_DIR) / "shaders";
    void Shader::addModule(const GLenum moduleType, const char* moduleFileName) const {
        const GLuint shaderModuleID = glCreateShader(moduleType);
        const auto shaderFilePath = shaderDirectory / moduleFileName;
        const string shaderSource = readShaderModuleFile(shaderFilePath.c_str());
        const char* shaderCode = shaderSource.c_str();
        glShaderSource(shaderModuleID, 1, &shaderCode, nullptr);
        GLint success;
        glCompileShader(shaderModuleID);
        glGetShaderiv(shaderModuleID, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            cerr << "Failed to compile " << Utilities::printShaderType(moduleType) << " '" << moduleFileName << "'." << endl;
            Utilities::printShaderInfoLog(shaderModuleID);
            glDeleteShader(shaderModuleID);
        }
        glAttachShader(shaderID_, shaderModuleID);
        glDeleteShader(shaderModuleID);
    }


    void Shader::activateShader() const {
        linkShaderModules();
        validateShader();
    }


    void Shader::bindShader() const {
        glUseProgram(shaderID_);
    }


    void Shader::unbindShader() {
        glUseProgram(0);
    }


    void Shader::deleteShader() {
        if (shaderID_ != 0) {
            unbindShader();
            glDeleteProgram(shaderID_);
            shaderID_ = 0;
        }
    }


    Shader::~Shader() {
        deleteShader();
    }


    void Shader::linkShaderModules() const {
        GLint success;
        glLinkProgram(shaderID_);
        glGetProgramiv(shaderID_, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            cerr << "Failed to link shader modules." << endl;
            Utilities::printShaderInfoLog(shaderID_);
        }
    }


    void Shader::validateShader() const {
        GLint success;
        GLuint testVAO;
        glGenVertexArrays(1, &testVAO);
        glBindVertexArray(testVAO);
        glValidateProgram(shaderID_);
        glGetProgramiv(shaderID_, GL_VALIDATE_STATUS, &success);
        if (success == GL_FALSE) {
            cerr << "Failed to validate shader." << endl;
            Utilities::printShaderInfoLog(shaderID_);
        }
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &testVAO);
    }


    GLint Shader::getUniformLocation(const char* name) {
        const string key(name);
        if (uniformLocations_.contains(key)) {
            return uniformLocations_[key];
        }
        const GLint location = glGetUniformLocation(shaderID_, name);
        if (location == -1) {
            cerr << "Failed to get uniform location " << name << "." << endl;
        }
        else {
            uniformLocations_[key] = location;
        }
        return location;
    }


    std::string Shader::readShaderModuleFile(const char* filePath) {
        ifstream fileStream(filePath, ios::in | ios::ate);
        if (!fileStream.is_open()) {
            cerr << "Failed to read file " << filePath << "." << endl;
            return "";
        }
        string fileContent;
        fileContent.reserve(fileStream.tellg());
        fileStream.seekg(0, ios::beg);
        fileContent.assign(
            istreambuf_iterator(fileStream),
            istreambuf_iterator<char>());
        parseIncludes(fileContent);
        return fileContent;
    }


    void Shader::parseIncludes(string& shaderModule) {
        unordered_set<string> includedFiles;
        vector<istringstream> sourceStack;
        sourceStack.emplace_back(shaderModule);
        stringstream finalOutput;
        while (!sourceStack.empty()) {
            istringstream& currentSource = sourceStack.back();
            string line;
            if (!std::getline(currentSource, line)) {
                sourceStack.pop_back();
                continue;
            }
            if (line.find("#include") == 0) {
                size_t firstQuote = line.find('\"');
                size_t lastQuote = line.find('\"', firstQuote + 1); // NOLINT
                if (firstQuote != string::npos && lastQuote != string::npos) {
                    string includeFileName = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                    path includePath = shaderDirectory / includeFileName;
                    if (includedFiles.contains(includePath.string())) {
                        cerr << "Warning: Shader #include-loop detected for " << includePath << endl;
                        continue;
                    }
                    if (ifstream includeFile(includePath); includeFile) {
                        stringstream includeContent;
                        includeContent << includeFile.rdbuf();
                        string includedShaderCode = includeContent.str();
                        sourceStack.emplace_back(includedShaderCode);
                        includedFiles.insert(includePath.string());
                    }
                    else cerr << "Failed to open shader include file: " << includePath << endl;
                }
            }
            else finalOutput << line << '\n';
        }
        shaderModule = finalOutput.str();
    }

} // namespace pixelforge::graphics
