#include <pixelforge/geometry/meshes/arrow.hpp>
#include <pixelforge/geometry/meshes/cone.hpp>
#include <pixelforge/geometry/meshes/cylinder.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::radians;


    Arrow::Arrow(const float shaftLength, const float shaftRadius, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->shaftLength_ = shaftLength;
        this->shaftRadius_ = shaftRadius;
        this->tipRadius_ = 2.0f * shaftRadius;
        this->tipLength_ = tipRadius_ / tan(radians(10.0f));
        this->sectorCount_ = sectorCount;
        generateMeshData();
    }


    void Arrow::calculatePositions() {
        std::vector<vec3> vertices;
        const Cylinder shaft(shaftRadius_, shaftLength_, sectorCount_);
        auto shaftVertices = shaft.getVertexData().getAttribute<vec3>(graphics::POSITION);
        vertices.insert(vertices.end(), shaftVertices.begin(), shaftVertices.end());
        const auto indexOffset = static_cast<uint32_t>(vertices.size());
        const Cone tip = Cone::ConeByRadius(tipLength_, tipRadius_, sectorCount_);
        auto tipVertices = tip.getVertexData().getAttribute<vec3>(graphics::POSITION);
        for (auto& vertex : tipVertices) {
            vertex.z += shaftLength_;
        }
        vertices.insert(vertices.end(), tipVertices.begin(), tipVertices.end());
        vertexData_.setAttribute(graphics::POSITION, vertices);

        vector<uint32_t> indices;
        auto shaftIndices = shaft.getIndices();
        indices.insert(indices.end(), shaftIndices.begin(), shaftIndices.end());
        for (const auto index : tip.getIndices()) {
            indices.push_back(index + indexOffset);
        }
        this->indices_ = indices;
    }


    void Arrow::calculateIndices() {
        // TODO: Extract Indices Calculation from other method
    }

} // namespace pixelforge::geometry
