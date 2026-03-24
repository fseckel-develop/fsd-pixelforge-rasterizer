#include <pixelforge/geometry/meshes/sphere_segment.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::radians;


    uint stackStart;
    uint stackEnd;


    SphereSegment::SphereSegment(const float radius, const float segmentStart, const float segmentEnd, const uint stackCount, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<uint>()) {
        this->radius_ = radius;
        this->segmentStart_ = segmentStart;
        this->segmentEnd_ = segmentEnd;
        this->stackCount_ = stackCount;
        this->sectorCount_ = sectorCount;
        generateMeshData();
        Mesh::setupMesh();
    }


    void SphereSegment::calculatePositions() {
        vector<vec3> vertices;
        stackStart = 0, stackEnd = 0;
        const float sectorStep = radians(360.0f / static_cast<float>(sectorCount_));
        const float stackStep = radians(180.0f / static_cast<float>(stackCount_));
        const float zStart = segmentStart_ * radius_;
        const float zEnd = segmentEnd_ * radius_; // NOLINT
        if (zStart < zEnd) {
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                vertices.push_back({vec3(0.0f, 0.0f, zStart)});
            }
            stackStart++;
            const float radiusStart = sqrt(radius_ * radius_ - zStart * zStart);
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                const float sectorAngle = static_cast<float>(sector) * sectorStep;
                const float x = -radiusStart * cos(sectorAngle);
                const float y = -radiusStart * sin(sectorAngle);
                vertices.push_back({vec3(x, y, zStart)});
            }
            stackStart++;
            for (uint stack = 0; stack <= stackCount_; stack++) {
                const float stackAngle = static_cast<float>(stack) * stackStep;
                const float z = -radius_ * cos(stackAngle);
                if (zStart < z && z < zEnd) {
                    const float radiusZ = -radius_ * sin(stackAngle);
                    for (uint sector = 0; sector <= sectorCount_; sector++) {
                        const float sectorAngle = static_cast<float>(sector) * sectorStep;
                        const float x = radiusZ * cos(sectorAngle);
                        const float y = radiusZ * sin(sectorAngle);
                        vertices.push_back({vec3(x, y, z)});
                    }
                    stackStart++;
                }
                else if (zEnd <= z) { break; }
            }
            const float radiusEnd = sqrt(radius_ * radius_ - zEnd * zEnd);
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                const float sectorAngle = static_cast<float>(sector) * sectorStep;
                const float x = -radiusEnd * cos(sectorAngle);
                const float y = -radiusEnd * sin(sectorAngle);
                vertices.push_back({vec3(x, y, zEnd)});
            }
            stackStart++;
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                vertices.push_back({vec3(0.0f, 0.0f, zEnd)});
            }
        }
        else if (zStart > zEnd) {
            bool segmentSegmentStarted = false;
            for (uint stack = 0; stack <= stackCount_; stack++) {
                const float stackAngle = static_cast<float>(stack) * stackStep;
                const float radiusZ = -radius_ * sin(stackAngle);
                const float z = -radius_ * cos(stackAngle);
                if (z < zEnd) {
                    for (uint sector = 0; sector <= sectorCount_; sector++) {
                        const float sectorAngle = static_cast<float>(sector) * sectorStep;
                        const float x = radiusZ * cos(sectorAngle);
                        const float y = radiusZ * sin(sectorAngle);
                        vertices.push_back({vec3(x, y, z)});
                    }
                    stackStart++;
                }
                else if (zEnd < z && z < zStart && !segmentSegmentStarted) {
                    const float radiusEnd = sqrt(radius_ * radius_ - zEnd * zEnd);
                    for (uint sector = 0; sector <= sectorCount_; sector++) {
                        const float sectorAngle = static_cast<float>(sector) * sectorStep;
                        const float x = -radiusEnd * cos(sectorAngle);
                        const float y = -radiusEnd * sin(sectorAngle);
                        vertices.push_back({vec3(x, y, zEnd)});
                    }
                    stackStart++;
                    for (uint sector = 0; sector <= sectorCount_; sector++) {
                        vertices.push_back({vec3(0.0f, 0.0f, zEnd)});
                    }
                    stackEnd = stackStart;
                    for (uint sector = 0; sector <= sectorCount_; sector++) {
                        vertices.push_back({vec3(0.0f, 0.0f, zStart)});
                    }
                    stackEnd++;
                    const float radiusStart = sqrt(radius_ * radius_ - zStart * zStart);
                    for (uint sector = 0; sector <= sectorCount_; sector++) {
                        const float sectorAngle = static_cast<float>(sector) * sectorStep;
                        const float x = -radiusStart * cos(sectorAngle);
                        const float y = -radiusStart * sin(sectorAngle);
                        vertices.push_back({vec3(x, y, zStart)});
                    }
                    stackEnd++;
                    segmentSegmentStarted = true;
                }
                else if (zStart < z) {
                    for (uint sector = 0; sector <= sectorCount_; sector++) {
                        const float sectorAngle = static_cast<float>(sector) * sectorStep;
                        const float x = radiusZ * cos(sectorAngle);
                        const float y = radiusZ * sin(sectorAngle);
                        vertices.push_back({vec3(x, y, z)});
                    }
                    stackEnd++;
                }
            }
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void SphereSegment::calculateIndices() {
        for (uint stack = 0; stack < stackStart; stack++) {
            for (uint sector = 0; sector < sectorCount_; sector++) {
                uint firstIndex = stack * (sectorCount_ + 1) + sector;
                uint secondIndex = firstIndex + sectorCount_ + 1;
                // first triangle
                indices_.push_back(firstIndex);
                indices_.push_back(secondIndex);
                indices_.push_back(firstIndex + 1);
                // second triangle
                indices_.push_back(secondIndex);
                indices_.push_back(secondIndex + 1);
                indices_.push_back(firstIndex + 1);
            }
        }
        for (uint stack = stackStart + 1; stack < stackEnd; stack++) {
            for (uint sector = 0; sector < sectorCount_; sector++) {
                uint firstIndex = stack * (sectorCount_ + 1) + sector;
                uint secondIndex = firstIndex + sectorCount_ + 1;
                // first triangle
                indices_.push_back(firstIndex);
                indices_.push_back(secondIndex);
                indices_.push_back(firstIndex + 1);
                // second triangle
                indices_.push_back(secondIndex);
                indices_.push_back(secondIndex + 1);
                indices_.push_back(firstIndex + 1);
            }
        }
    }

} // namespace pixelforge::geometry
