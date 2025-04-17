#include "ShaderProgram.h"
#include "../../Utilities.h"
#include <fstream>
#include <sstream>


ShaderProgram::ShaderProgram() {
    programID = glCreateProgram();
    if (!programID) {
        cerr << "Failed to create shader program." << endl;
    }
}


const filesystem::path shaderDirectory = "../Resources/Shaders";
void ShaderProgram::AddShader(const GLenum shaderType, const char* shaderFileName) const {
    const GLuint shaderID = glCreateShader(shaderType);
    const auto shaderFilePath = shaderDirectory / shaderFileName;
    const string shaderSource = ReadShaderFile(shaderFilePath.c_str());
    const char* shaderCode = shaderSource.c_str();
    glShaderSource(shaderID, 1, &shaderCode, nullptr);
    GLint success;
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        cerr << "Failed to compile " << Utilities::PrintShaderType(shaderType) << "." << endl;
        Utilities::PrintShaderInfoLog(shaderID);
        glDeleteShader(shaderID);
    }
    glAttachShader(programID, shaderID);
    glDeleteShader(shaderID);
}


void ShaderProgram::ActivateProgram() const {
    LinkProgram();
    ValidateProgram();
}


void ShaderProgram::UseProgram() const {
    glUseProgram(programID);
}


void ShaderProgram::UnuseProgram() {
    glUseProgram(0);
}


void ShaderProgram::DeleteProgram() {
    if (programID != 0) {
        UnuseProgram();
        glDeleteProgram(programID);
        programID = 0;
    }
}


ShaderProgram::~ShaderProgram() {
    DeleteProgram();
}


void ShaderProgram::LinkProgram() const {
    GLint success;
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        cerr << "Failed to link shader program." << endl;
        Utilities::PrintShaderInfoLog(programID);
    }
}


void ShaderProgram::ValidateProgram() const {
    GLint success;
    GLuint testVAO;
    glGenVertexArrays(1, &testVAO);
    glBindVertexArray(testVAO);
    glValidateProgram(programID);
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE) {
        cerr << "Failed to validate shader program." << endl;
        Utilities::PrintShaderInfoLog(programID);
    }
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &testVAO);
}


GLint ShaderProgram::GetUniformLocation(const char* name) {
    const string key(name);
    if (uniformLocations.contains(key)) {
        return uniformLocations[key];
    }
    const GLint location = glGetUniformLocation(programID, name);
    if (location == -1) {
        cerr << "Failed to get uniform location " << name << "." << endl;
    }
    else {
        uniformLocations[key] = location;
    }
    return location;
}


std::string ShaderProgram::ReadShaderFile(const char* filePath) {
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


void ShaderProgram::ParseIncludes(string& shaderSource) {
    unordered_set<string> includedFiles;
    vector<istringstream> sourceStack;
    sourceStack.emplace_back(shaderSource);
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
            size_t lastQuote = line.find('\"', firstQuote + 1);
            if (firstQuote != string::npos && lastQuote != string::npos) {
                string includeFileName = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                filesystem::path includePath = shaderDirectory / includeFileName;
                if (includedFiles.contains(includePath.string())) {
                    cerr << "Warning: Shader include loop detected for " << includePath << endl;
                    continue;
                }
                ifstream includeFile(includePath);
                if (includeFile) {
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
    shaderSource = finalOutput.str();
}
