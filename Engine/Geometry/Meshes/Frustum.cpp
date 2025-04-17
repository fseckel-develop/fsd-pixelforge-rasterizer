#include "Frustum.h"


Frustum::Frustum(const float radiusBase, const float radiusTop, const float height, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->radiusBase = radiusBase;
    this->radiusTop = radiusTop;
    this->height = height;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Frustum::CalculatePositions() {
    vector<vec3> vertices;
    const float sectorStep = 360.0f / static_cast<float>(sectorCount);
    for (uint sector = 0; sector <= sectorCount; sector++) {
        vertices.push_back({vec3(0.0f, 0.0f, 0.0f)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
        const float x = radiusBase * cos(sectorAngle);
        const float y = radiusBase * sin(sectorAngle);
        vertices.push_back({vec3(x, y, 0.0f)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
        const float x = radiusTop * cos(sectorAngle);
        const float y = radiusTop * sin(sectorAngle);
        vertices.push_back({vec3(x, y, height)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        vertices.push_back({vec3(0.0f, 0.0f, height)});
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void Frustum::CalculateIndices() {
    indices.clear();
    for (uint stack = 0; stack < 3; stack++) {
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
