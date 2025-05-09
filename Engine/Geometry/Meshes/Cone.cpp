#include "Cone.h"
using namespace std; using namespace glm;


Cone Cone::ConeByRadius(const float height, const float baseRadius, const uint sectorCount) {
    const float halfAngle = atan(baseRadius / height);
    return Cone(height, baseRadius, halfAngle, sectorCount);
}


Cone Cone::ConeByAngle(const float height, float halfAngle, const uint sectorCount) {
    if (halfAngle > 85.0f) halfAngle = 85.0f;
    const float radiusBase = height * tan(radians(halfAngle));
    return Cone(height, radiusBase, halfAngle, sectorCount);
}


Cone::Cone(const float height, const float baseRadius, const float halfAngle, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->height = height;
    this->baseRadius = baseRadius;
    this->halfAngle = halfAngle;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Cone::CalculatePositions() {
    vector<vec3> vertices;
    const float sectorStep = 360.0f / static_cast<float>(sectorCount);
    for (uint sector = 0; sector <= sectorCount; sector++) {
        vertices.push_back({vec3(0.0f, 0.0f, 0.0f)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
        const float x = baseRadius * cos(sectorAngle);
        const float y = baseRadius * sin(sectorAngle);
        vertices.push_back({vec3(x, y, 0.0f)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        vertices.push_back({vec3(0.0f, 0.0f, height)});
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void Cone::CalculateIndices() {
    indices.clear();
    for (uint stack = 0; stack < 2; stack++) {
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
