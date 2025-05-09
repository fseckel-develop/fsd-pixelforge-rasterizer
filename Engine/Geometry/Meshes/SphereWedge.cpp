#include "SphereWedge.h"
using namespace std; using namespace glm;


SphereWedge::SphereWedge(const float radius, const float wedgeAngle, const uint stackCount, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->radius = radius;
    this->wedgeAngle = wedgeAngle;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void SphereWedge::CalculatePositions() {
    vector<vec3> vertices;
    const float sectorStep = radians(360.0f / static_cast<float>(sectorCount));
    const float stackStep = radians(180.0f / static_cast<float>(stackCount));
    for (uint stack = 0; stack <= stackCount; stack++) {
        const float stackAngle = static_cast<float>(stack) * stackStep;
        const float radiusZ = radius * sin(stackAngle);
        const float z = radius * cos(stackAngle);
        for (uint sector = 0; sector <= sectorCount; sector++) {
            const float sectorAngle = static_cast<float>(sector) * sectorStep; // NOLINT
            if (sectorAngle < radians(wedgeAngle)) {
                const float x = radiusZ * cos(sectorAngle);
                const float y = radiusZ * sin(sectorAngle);
                vertices.push_back({vec3(x, y, z)});
            }
        }
        if (wedgeAngle < 360.0f) {
            const float x = radiusZ * cos(radians(wedgeAngle));
            const float y = radiusZ * sin(radians(wedgeAngle));
            vertices.push_back({vec3(x, y, z)});
            vertices.push_back({vec3(0.0f, 0.0f, z)});
        }
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void SphereWedge::CalculateIndices() {
    indices.clear();
    const uint sectorMax = static_cast<int>(ceil(wedgeAngle / 360.0f * static_cast<float>(sectorCount))) + 1;
    for (uint stack = 0; stack < stackCount; stack++) {
        for (uint sector = 0; sector <= sectorMax; sector++) {
            uint firstIndex = stack * (sectorMax + 1) + sector;
            uint secondIndex = firstIndex + sectorMax + 1;
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
