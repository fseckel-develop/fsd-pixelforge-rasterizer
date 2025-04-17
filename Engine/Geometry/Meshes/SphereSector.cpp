#include "SphereSector.h"
#include <ostream>


SphereSector::SphereSector(const float radius, const float angle, const uint stackCount, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->radius = radius;
    this->openingAngle = angle;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void SphereSector::CalculatePositions() {
    vector<vec3> vertices;
    const float sectorStep = radians(360.0f / static_cast<float>(sectorCount));
    const float stackStep = radians(180.0f / static_cast<float>(stackCount));
    for (uint stack = 0; stack <= stackCount; stack++) {
        const float stackAngle = static_cast<float>(stack) * stackStep; // NOLINT
        if (stackAngle < radians(openingAngle / 2.0f)) {
            const float radiusZ = radius * sin(stackAngle);
            const float z = radius * cos(stackAngle);
            for (uint sector = 0; sector <= sectorCount; sector++) {
                const float sectorAngle = static_cast<float>(sector) * sectorStep;
                const float x = radiusZ * cos(sectorAngle);
                const float y = radiusZ * sin(sectorAngle);
                vertices.push_back({vec3(x, y, z)});
            }
        }
    }
    if (openingAngle < 360.0f) {
        const float radiusZ = radius * sin(radians(openingAngle) / 2.0f);
        const float z = radius * cos(radians(openingAngle) / 2.0f);
        for (uint sector = 0; sector <= sectorCount; sector++) {
            const float sectorAngle = static_cast<float>(sector) * sectorStep;
            const float x = radiusZ * cos(sectorAngle);
            const float y = radiusZ * sin(sectorAngle);
            vertices.push_back({vec3(x, y, z)});
        }
        for (uint sector = 0; sector <= sectorCount; sector++) {
            vertices.push_back({vec3(0.0f, 0.0f, 0.0f)});
        }
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void SphereSector::CalculateIndices() {
    indices.clear();
    for (uint stack = 0; stack < stackCount; stack++) {
        for (uint sector = 0; sector < sectorCount; sector++) {
            uint firstIndex = stack * (sectorCount + 1) + sector;
            uint secondIndex = firstIndex + sectorCount + 1;
            /* first triangle */
            indices.push_back(firstIndex);
            indices.push_back(secondIndex);
            indices.push_back(firstIndex + 1);
            /* second triangle */
            indices.push_back(secondIndex);
            indices.push_back(secondIndex + 1);
            indices.push_back(firstIndex + 1);
        }
    }
}
