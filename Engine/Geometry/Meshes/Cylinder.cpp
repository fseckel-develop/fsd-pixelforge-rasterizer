#include "Cylinder.h"
using namespace std; using namespace glm;


Cylinder::Cylinder(const float radius, const float height, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->radius = radius;
    this->height = height;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Cylinder::CalculatePositions() {
    vector<vec3> vertices;
    const float sectorStep = 360.0f / static_cast<float>(sectorCount);
    for (uint sector = 0; sector <= sectorCount; sector++) {
        vertices.push_back({vec3(0.0f, 0.0f, 0.0f)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
        const float x = radius * cos(sectorAngle);
        const float y = radius * sin(sectorAngle);
        vertices.push_back({vec3(x, y, 0.0f)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
        const float x = radius * cos(sectorAngle);
        const float y = radius * sin(sectorAngle);
        vertices.push_back({vec3(x, y, height)});
    }
    for (uint sector = 0; sector <= sectorCount; sector++) {
        vertices.push_back({vec3(0.0f, 0.0f, height)});
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void Cylinder::CalculateIndices() {
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
