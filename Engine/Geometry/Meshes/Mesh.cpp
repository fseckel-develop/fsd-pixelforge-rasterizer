#include "Mesh.h"
#include "../../Graphics/Pipeline/VertexArray.h"
#include "../../Graphics/Pipeline/VertexBuffer.h"
#include "../../Graphics/Pipeline/IndexBuffer.h"
#include <fstream>
#include <iostream>
#include <sstream>


Mesh::Mesh(VertexData vertexData, const vector<GLuint>& indices) {
    this->vertexData = std::move(vertexData);
    this->indices = indices;
    Mesh::SetupMesh();
}


Mesh::Mesh(const string& filePath, const bool invertY) {
    ParseOBJ(filePath, invertY);
    Mesh::SetupMesh();
}


const VertexArray* Mesh::GetVAO() const {
    return VAO;
}


const VertexData& Mesh::GetVertexData() const {
    return vertexData;
}


const vector<GLuint>& Mesh::GetIndices() const {
    return indices;
}


GLsizei Mesh::GetVertexCount() const {
    return vertexCount;
}


void Mesh::Render() const {
    VAO->BindVAO();
    if (indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    } else {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    }
    VertexArray::UnbindVAO();
}


void Mesh::SetupMesh() {
    if (!vertexData.HasAttribute(POSITION)) return;
    VAO = new VertexArray();
    const auto VBO = VertexBuffer(vertexData);
    VAO->AddVertexBuffer(VBO);
    const auto IBO = IndexBuffer(indices);
    VAO->SetIndexBuffer(IBO);
}


void Mesh::GenerateMeshData() {
    vertexData.Clear();
    CalculatePositions();
    CalculateTextureCoordinates();
    CalculateNormals();
    CalculateColors();
    CalculateIndices();
}


void Mesh::UpdateMesh() {
    GenerateMeshData();
    VAO->DeleteVAO();
    SetupMesh();
}


Mesh::~Mesh() {
    VAO->DeleteVAO();
}


const filesystem::path modelDirectory = "../Resources/Models";
void Mesh::ParseOBJ(const string& modelFileName, const bool invertY) {
    const auto modelFilePath = modelDirectory / modelFileName;
    ifstream fileStream(modelFilePath, ios::in);
    vector<vec3> positions, pos;
    vector<vec2> textureCoordinates, tex;
    vector<vec3> normals, nor;
    string line;
    while (!fileStream.eof()) {
        getline(fileStream, line);
        if (line.compare(0, 2, "v ") == 0) {
            vec3 position;
            stringstream ss(line.erase(0, 1));
            ss >> position.x; ss >> position.y; ss >> position.z;
            pos.push_back(position);
        }
        if (line.compare(0, 2, "vt") == 0) {
            vec2 textureCoordinate;
            stringstream ss(line.erase(0, 2));
            ss >> textureCoordinate.x; ss >> textureCoordinate.y;
            if (invertY) {
                textureCoordinate.y = 1.0f - textureCoordinate.y;
            }
            tex.push_back(textureCoordinate);
        }
        if (line.compare(0, 2, "vn") == 0) {
            vec3 normal;
            stringstream ss(line.erase(0, 2));
            ss >> normal.x; ss >> normal.y; ss >> normal.z;
            nor.push_back(normal);
        }
        if (line.compare(0, 2, "f ") == 0) {
            string corner, v, t, n;
            stringstream ss(line.erase(0, 2));
            for (int i = 0; i < 3; i++) {
                getline(ss, corner, ' ');
                stringstream cornerSS(corner);
                getline(cornerSS, v, '/');
                getline(cornerSS, t, '/');
                getline(cornerSS, n, '/');
                int vertexRef = stoi(v) - 1;
                int texCoordsRef = stoi(t) - 1;
                int normalRef = stoi(n) - 1;
                positions.push_back(pos[vertexRef]);
                textureCoordinates.push_back(tex[texCoordsRef]);
                normals.push_back(nor[normalRef]);
            }
        }
    }
    vertexData.SetAttribute(POSITION, positions);
    vertexData.SetAttribute(TEXTURE, textureCoordinates);
    vertexData.SetAttribute(NORMAL, normals);
    vertexCount = static_cast<GLsizei>(positions.size());
}
