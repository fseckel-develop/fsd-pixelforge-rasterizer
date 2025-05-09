#include "SphereSegment.h"
using namespace std; using namespace glm;


uint stackStart;
uint stackEnd;


SphereSegment::SphereSegment(const float radius, const float segmentStart, const float segmentEnd, const uint stackCount, const uint sectorCount):
    Mesh(VertexData(), vector<uint>()) {
    this->radius = radius;
    this->segmentStart = segmentStart;
    this->segmentEnd = segmentEnd;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void SphereSegment::CalculatePositions() {
    vector<vec3> vertices;
    stackStart = 0, stackEnd = 0;
    const float sectorStep = radians(360.0f / static_cast<float>(sectorCount));
    const float stackStep = radians(180.0f / static_cast<float>(stackCount));
    const float zStart = segmentStart * radius;
    const float zEnd = segmentEnd * radius; // NOLINT
    if (zStart < zEnd) {
        for (uint sector = 0; sector <= sectorCount; sector++) {
            vertices.push_back({vec3(0.0f, 0.0f, zStart)});
        }
        stackStart++;
        const float radiusStart = sqrt(radius * radius - zStart * zStart);
        for (uint sector = 0; sector <= sectorCount; sector++) {
            const float sectorAngle = static_cast<float>(sector) * sectorStep;
            const float x = -radiusStart * cos(sectorAngle);
            const float y = -radiusStart * sin(sectorAngle);
            vertices.push_back({vec3(x, y, zStart)});
        }
        stackStart++;
        for (uint stack = 0; stack <= stackCount; stack++) {
            const float stackAngle = static_cast<float>(stack) * stackStep;
            const float z = -radius * cos(stackAngle);
            if (zStart < z && z < zEnd) {
                const float radiusZ = -radius * sin(stackAngle);
                for (uint sector = 0; sector <= sectorCount; sector++) {
                    const float sectorAngle = static_cast<float>(sector) * sectorStep;
                    const float x = radiusZ * cos(sectorAngle);
                    const float y = radiusZ * sin(sectorAngle);
                    vertices.push_back({vec3(x, y, z)});
                }
                stackStart++;
            }
            else if (zEnd <= z) { break; }
        }
        const float radiusEnd = sqrt(radius * radius - zEnd * zEnd);
        for (uint sector = 0; sector <= sectorCount; sector++) {
            const float sectorAngle = static_cast<float>(sector) * sectorStep;
            const float x = -radiusEnd * cos(sectorAngle);
            const float y = -radiusEnd * sin(sectorAngle);
            vertices.push_back({vec3(x, y, zEnd)});
        }
        stackStart++;
        for (uint sector = 0; sector <= sectorCount; sector++) {
            vertices.push_back({vec3(0.0f, 0.0f, zEnd)});
        }
    }
    else if (zStart > zEnd) {
        bool segmentSegmentStarted = false;
        for (uint stack = 0; stack <= stackCount; stack++) {
            const float stackAngle = static_cast<float>(stack) * stackStep;
            const float radiusZ = -radius * sin(stackAngle);
            const float z = -radius * cos(stackAngle);
            if (z < zEnd) {
                for (uint sector = 0; sector <= sectorCount; sector++) {
                    const float sectorAngle = static_cast<float>(sector) * sectorStep;
                    const float x = radiusZ * cos(sectorAngle);
                    const float y = radiusZ * sin(sectorAngle);
                    vertices.push_back({vec3(x, y, z)});
                }
                stackStart++;
            }
            else if (zEnd < z && z < zStart && !segmentSegmentStarted) {
                const float radiusEnd = sqrt(radius * radius - zEnd * zEnd);
                for (uint sector = 0; sector <= sectorCount; sector++) {
                    const float sectorAngle = static_cast<float>(sector) * sectorStep;
                    const float x = -radiusEnd * cos(sectorAngle);
                    const float y = -radiusEnd * sin(sectorAngle);
                    vertices.push_back({vec3(x, y, zEnd)});
                }
                stackStart++;
                for (uint sector = 0; sector <= sectorCount; sector++) {
                    vertices.push_back({vec3(0.0f, 0.0f, zEnd)});
                }
                stackEnd = stackStart;
                for (uint sector = 0; sector <= sectorCount; sector++) {
                    vertices.push_back({vec3(0.0f, 0.0f, zStart)});
                }
                stackEnd++;
                const float radiusStart = sqrt(radius * radius - zStart * zStart);
                for (uint sector = 0; sector <= sectorCount; sector++) {
                    const float sectorAngle = static_cast<float>(sector) * sectorStep;
                    const float x = -radiusStart * cos(sectorAngle);
                    const float y = -radiusStart * sin(sectorAngle);
                    vertices.push_back({vec3(x, y, zStart)});
                }
                stackEnd++;
                segmentSegmentStarted = true;
            }
            else if (zStart < z) {
                for (uint sector = 0; sector <= sectorCount; sector++) {
                    const float sectorAngle = static_cast<float>(sector) * sectorStep;
                    const float x = radiusZ * cos(sectorAngle);
                    const float y = radiusZ * sin(sectorAngle);
                    vertices.push_back({vec3(x, y, z)});
                }
                stackEnd++;
            }
        }
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void SphereSegment::CalculateIndices() {
    for (uint stack = 0; stack < stackStart; stack++) {
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
    for (uint stack = stackStart + 1; stack < stackEnd; stack++) {
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
