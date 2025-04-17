#include "Torus.h"


Torus::Torus(const float innerRadius, const float torusRadius, const uint stackCount, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->innerRadius = innerRadius;
    this->torusRadius = torusRadius;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Torus::CalculatePositions() {
    vector<vec3> vertices;
    const float sectorStep = radians(360.0f / static_cast<float>(sectorCount));
    const float stackStep = radians(360.0f / static_cast<float>(stackCount));
    for (uint stack = 0; stack <= stackCount; stack++) {
        const float stackAngle = static_cast<float>(stack) * stackStep;
        const float dx = cos(stackAngle) * torusRadius;
        const float z = sin(stackAngle) * torusRadius;
        const float radiusZ = innerRadius + torusRadius - dx;
        for (uint sector = 0; sector <= sectorCount; sector++) {
            const float sectorAngle = static_cast<float>(sector) * sectorStep;
            const float x = radiusZ * cos(sectorAngle);
            const float y = radiusZ * sin(sectorAngle);
            vertices.push_back({vec3(x, y, z)});
        }
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void Torus::CalculateIndices() {
    indices.clear();
    for (uint stack = 0; stack < stackCount; stack++) {
        for (uint sector = 0; sector < sectorCount; sector++) {
            uint firstIndex = stack * (sectorCount + 1) + sector;
            uint secondIndex = firstIndex + sectorCount + 1;
            // first triangle
            indices.push_back(firstIndex);
            indices.push_back(secondIndex);
            indices.push_back(firstIndex + 1);
            // second triangle
            indices.push_back(secondIndex);
            indices.push_back(secondIndex + 1);
            indices.push_back(firstIndex + 1);
        }
    }
}
