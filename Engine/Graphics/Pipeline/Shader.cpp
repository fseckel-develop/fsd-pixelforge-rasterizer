#include "Shader.h"
#include "../../Utilities.h"
#include <fstream>
#include <sstream>
using namespace std;


Shader::Shader() {
    shaderID = glCreateProgram();
    if (!shaderID) {
        cerr << "Failed to create shader program." << endl;
    }
}


const filesystem::path shaderDirectory = "../Resources/Shaders";
void Shader::AddModule(const GLenum moduleType, const char* moduleFileName) const {
    const GLuint shaderModuleID = glCreateShader(moduleType);
    const auto shaderFilePath = shaderDirectory / moduleFileName;
    const string shaderSource = ReadShaderModuleFile(shaderFilePath.c_str());
    const char* shaderCode = shaderSource.c_str();
    glShaderSource(shaderModuleID, 1, &shaderCode, nullptr);
    GLint success;
    glCompileShader(shaderModuleID);
    glGetShaderiv(shaderModuleID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        cerr << "Failed to compile " << Utilities::PrintShaderType(moduleType) << " '" << moduleFileName << "'." << endl;
        Utilities::PrintShaderInfoLog(shaderModuleID);
        glDeleteShader(shaderModuleID);
    }
    glAttachShader(shaderID, shaderModuleID);
    glDeleteShader(shaderModuleID);
}


void Shader::ActivateShader() const {
    LinkShaderModules();
    ValidateShader();
}


void Shader::BindShader() const {
    glUseProgram(shaderID);
}


void Shader::UnbindShader() {
    glUseProgram(0);
}


void Shader::DeleteShader() {
    if (shaderID != 0) {
        UnbindShader();
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
}


Shader::~Shader() {
    DeleteShader();
}


void Shader::LinkShaderModules() const {
    GLint success;
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        cerr << "Failed to link shader modules." << endl;
        Utilities::PrintShaderInfoLog(shaderID);
    }
}


void Shader::ValidateShader() const {
    GLint success;
    GLuint testVAO;
    glGenVertexArrays(1, &testVAO);
    glBindVertexArray(testVAO);
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE) {
        cerr << "Failed to validate shader." << endl;
        Utilities::PrintShaderInfoLog(shaderID);
    }
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &testVAO);
}


GLint Shader::GetUniformLocation(const char* name) {
    const string key(name);
    if (uniformLocations.contains(key)) {
        return uniformLocations[key];
    }
    const GLint location = glGetUniformLocation(shaderID, name);
    if (location == -1) {
        cerr << "Failed to get uniform location " << name << "." << endl;
    }
    else {
        uniformLocations[key] = location;
    }
    return location;
}


std::string Shader::ReadShaderModuleFile(const char* filePath) {
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
    ParseIncludes(fileContent);
    return fileContent;
}


void Shader::ParseIncludes(string& shaderModule) {
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
                filesystem::path includePath = shaderDirectory / includeFileName;
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
