#include <pixelforge/geometry/meshes/mesh.hpp>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "graphics/pipeline/vertex_array.hpp"
#include "graphics/pipeline/vertex_buffer.hpp"
#include "graphics/pipeline/index_buffer.hpp"


namespace pixelforge::geometry {

    using std::vector;
    using std::string;
    using std::shared_ptr;
    using std::make_shared;
    using std::filesystem::path;
    using std::ifstream;
    using std::stringstream;
    using std::ios;
    using glm::vec2;
    using glm::vec3;


    Mesh::Mesh(graphics::VertexData vertexData, const vector<GLuint>& indices) {
        this->vertexData_ = std::move(vertexData);
        this->indices_ = indices;
    }


    Mesh::Mesh(const string& fileName, const bool invertY) {
        parseOBJ(fileName, invertY);
    }


    const shared_ptr<graphics::VertexArray>& Mesh::getVAO() const {
        return VAO_;
    }


    const graphics::VertexData& Mesh::getVertexData() const {
        return vertexData_;
    }


    const vector<GLuint>& Mesh::getIndices() const {
        return indices_;
    }


    GLsizei Mesh::getVertexCount() const {
        return vertexData_.getVertexCount();
    }


    void Mesh::uploadToGPU() {
        if (uploadedToGPU_ || !vertexData_.hasAttribute(graphics::POSITION)) return;
        VAO_ = make_shared<graphics::VertexArray>();
        const auto VBO = graphics::VertexBuffer(vertexData_);
        VAO_->addVertexBuffer(VBO);
        auto IBO = graphics::IndexBuffer(indices_);
        VAO_->setIndexBuffer(IBO);
        uploadedToGPU_ = true;
    }


    void Mesh::generateMeshData() {
        vertexData_.clear();
        calculatePositions();
        calculateTextureCoordinates();
        calculateNormals();
        calculateColors();
        calculateIndices();
    }


    void Mesh::updateMesh() {
        generateMeshData();
        if (uploadedToGPU_) {
            if (VAO_) {
                VAO_->deleteVAO();
            }
            VAO_.reset();
            uploadedToGPU_ = false;
            uploadToGPU();
        }
    }


    const path modelDirectory = path(PIXELFORGE_ASSET_DIR) / "models";
    void Mesh::parseOBJ(const string& fileName, const bool invertY) {
        const auto modelFilePath = modelDirectory / fileName;
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
        vertexData_.setAttribute(graphics::POSITION, positions);
        vertexData_.setAttribute(graphics::TEXTURE, textureCoordinates);
        vertexData_.setAttribute(graphics::NORMAL, normals);
    }

} // namespace pixelforge::geometry
