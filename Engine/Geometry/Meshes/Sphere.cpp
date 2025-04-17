#include "Sphere.h"


Sphere::Sphere(const float radius, const uint precision):
    Mesh(VertexData(), vector<GLuint>()) {
    this->radius = radius;
    this->precision = precision;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Sphere::CalculatePositions() {
    vector<vec3> positions;
    positions.reserve((precision + 1) * (precision + 1));
    const float sectorStep = radians(360.0f / static_cast<float>(precision));
    const float stackStep = radians(180.0f / static_cast<float>(precision));
    for (uint stack = 0; stack <= precision; stack++) {
        const float stackAngle = static_cast<float>(stack) * stackStep;
        const float radiusY = radius * sin(stackAngle);
        const float y = radius * cos(stackAngle);
        for (uint sector = 0; sector <= precision; sector++) {
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
    textureCoordinates.reserve((precision + 1) * (precision + 1));
    for (uint stack = 0; stack <= precision; stack++) {
        for (uint sector = 0; sector <= precision; sector++) {
            const float u = static_cast<float>(sector) / static_cast<float>(precision);
            const float v = static_cast<float>(stack) / static_cast<float>(precision);
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
    for (uint stack = 0; stack < precision; stack++) {
        for (uint sector = 0; sector < precision; sector++) {
            uint firstIndex = stack * (precision + 1) + sector;
            uint secondIndex = firstIndex + precision + 1;
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
