#include <pixelforge/geometry/meshes/sphere_sector.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::radians;


    SphereSector::SphereSector(const float radius, const float angle, const uint stackCount, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->radius_ = radius;
        this->openingAngle_ = angle;
        this->stackCount_ = stackCount;
        this->sectorCount_ = sectorCount;
        generateMeshData();
    }


    void SphereSector::calculatePositions() {
        vector<vec3> vertices;
        const float sectorStep = radians(360.0f / static_cast<float>(sectorCount_));
        const float stackStep = radians(180.0f / static_cast<float>(stackCount_));
        for (uint stack = 0; stack <= stackCount_; stack++) {
            const float stackAngle = static_cast<float>(stack) * stackStep; // NOLINT
            if (stackAngle < radians(openingAngle_ / 2.0f)) {
                const float radiusZ = radius_ * sin(stackAngle);
                const float z = radius_ * cos(stackAngle);
                for (uint sector = 0; sector <= sectorCount_; sector++) {
                    const float sectorAngle = static_cast<float>(sector) * sectorStep;
                    const float x = radiusZ * cos(sectorAngle);
                    const float y = radiusZ * sin(sectorAngle);
                    vertices.push_back({vec3(x, y, z)});
                }
            }
        }
        if (openingAngle_ < 360.0f) {
            const float radiusZ = radius_ * sin(radians(openingAngle_) / 2.0f);
            const float z = radius_ * cos(radians(openingAngle_) / 2.0f);
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                const float sectorAngle = static_cast<float>(sector) * sectorStep;
                const float x = radiusZ * cos(sectorAngle);
                const float y = radiusZ * sin(sectorAngle);
                vertices.push_back({vec3(x, y, z)});
            }
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                vertices.push_back({vec3(0.0f, 0.0f, 0.0f)});
            }
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void SphereSector::calculateIndices() {
        indices_.clear();
        for (uint stack = 0; stack < stackCount_; stack++) {
            for (uint sector = 0; sector < sectorCount_; sector++) {
                uint firstIndex = stack * (sectorCount_ + 1) + sector;
                uint secondIndex = firstIndex + sectorCount_ + 1;
                /* first triangle */
                indices_.push_back(firstIndex);
                indices_.push_back(secondIndex);
                indices_.push_back(firstIndex + 1);
                /* second triangle */
                indices_.push_back(secondIndex);
                indices_.push_back(secondIndex + 1);
                indices_.push_back(firstIndex + 1);
            }
        }
    }

} // namespace pixelforge::geometry
