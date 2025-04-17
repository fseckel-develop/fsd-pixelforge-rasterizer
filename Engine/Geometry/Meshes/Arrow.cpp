#include "Arrow.h"
#include "Cone.h"
#include "Cylinder.h"


Arrow::Arrow(const float shaftLength, const float shaftRadius, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->shaftLength = shaftLength;
    this->shaftRadius = shaftRadius;
    this->tipRadius = 2.0f * shaftRadius;
    this->tipLength = tipRadius / tan(radians(10.0f));
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Arrow::CalculatePositions() {
    std::vector<vec3> vertices;
    Cylinder shaft(shaftRadius, shaftLength, sectorCount);
    auto shaftVertices = shaft.GetVertexData().GetAttribute<vec3>(POSITION);
    vertices.insert(vertices.end(), shaftVertices.begin(), shaftVertices.end());
    const auto indexOffset = static_cast<uint32_t>(vertices.size());
    Cone tip = Cone::ConeByRadius(tipLength, tipRadius, sectorCount);
    auto tipVertices = tip.GetVertexData().GetAttribute<vec3>(POSITION);
    for (auto& vertex : tipVertices) {
        vertex.z += shaftLength;
    }
    vertices.insert(vertices.end(), tipVertices.begin(), tipVertices.end());
    vertexData.SetAttribute(POSITION, vertices);

    vector<uint32_t> indices;
    auto shaftIndices = shaft.GetIndices();
    indices.insert(indices.end(), shaftIndices.begin(), shaftIndices.end());
    const auto coneIndices = tip.GetIndices();
    for (const auto index : coneIndices) {
        indices.push_back(index + indexOffset);
    }
    this->indices = indices;
}


void Arrow::CalculateIndices() {
// TODO: Extract Indices Calculation from other method
}
