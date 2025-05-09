#include "Sphere.h"
using namespace std; using namespace glm;


Sphere::Sphere(const float radius, const uint stackCount, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->radius = radius;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Sphere::CalculatePositions() {
    vector<vec3> positions;
    positions.reserve((stackCount + 1) * (sectorCount + 1));
    const float sectorStep = radians(360.0f / static_cast<float>(sectorCount));
    const float stackStep = radians(180.0f / static_cast<float>(stackCount));
    for (uint stack = 0; stack <= stackCount; stack++) {
        const float stackAngle = static_cast<float>(stack) * stackStep;
        const float radiusY = radius * sin(stackAngle);
        const float y = radius * cos(stackAngle);
        for (uint sector = 0; sector <= sectorCount; sector++) {
            const float sectorAngle = static_cast<float>(sector) * sectorStep;
            const float x = radiusY * sin(sectorAngle);
            const float z = radiusY * cos(sectorAngle);
            positions.push_back({vec3(x, y, z)});
        }
    }
    vertexData.SetAttribute(POSITION, positions);
}


void Sphere::CalculateTextureCoordinates() {
    vector<vec2> textureCoordinates;
    textureCoordinates.reserve((stackCount + 1) * (sectorCount + 1));
    for (uint stack = 0; stack <= stackCount; stack++) {
        for (uint sector = 0; sector <= sectorCount; sector++) {
            const float u = static_cast<float>(sector) / static_cast<float>(sectorCount);
            const float v = static_cast<float>(stack) / static_cast<float>(stackCount);
            textureCoordinates.push_back({vec2(u, v)});
        }
    }
    vertexData.SetAttribute(TEXTURE, textureCoordinates);
}


void Sphere::CalculateNormals() {
    vector<vec3> normals = vertexData.GetAttribute<vec3>(POSITION);
    for (auto& normal : normals) {
        normal = normalize(normal);
    }
    vertexData.SetAttribute(NORMAL, normals);
}


void Sphere::CalculateIndices() {
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
