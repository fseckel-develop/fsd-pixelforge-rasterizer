#include <pixelforge/geometry/meshes/sphere_wedge.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::radians;


    SphereWedge::SphereWedge(const float radius, const float wedgeAngle, const uint stackCount, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->radius_ = radius;
        this->wedgeAngle_ = wedgeAngle;
        this->stackCount_ = stackCount;
        this->sectorCount_ = sectorCount;
        generateMeshData();
    }


    void SphereWedge::calculatePositions() {
        vector<vec3> vertices;
        const float sectorStep = radians(360.0f / static_cast<float>(sectorCount_));
        const float stackStep = radians(180.0f / static_cast<float>(stackCount_));
        for (uint stack = 0; stack <= stackCount_; stack++) {
            const float stackAngle = static_cast<float>(stack) * stackStep;
            const float radiusZ = radius_ * sin(stackAngle);
            const float z = radius_ * cos(stackAngle);
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                const float sectorAngle = static_cast<float>(sector) * sectorStep; // NOLINT
                if (sectorAngle < radians(wedgeAngle_)) {
                    const float x = radiusZ * cos(sectorAngle);
                    const float y = radiusZ * sin(sectorAngle);
                    vertices.push_back({vec3(x, y, z)});
                }
            }
            if (wedgeAngle_ < 360.0f) {
                const float x = radiusZ * cos(radians(wedgeAngle_));
                const float y = radiusZ * sin(radians(wedgeAngle_));
                vertices.push_back({vec3(x, y, z)});
                vertices.push_back({vec3(0.0f, 0.0f, z)});
            }
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void SphereWedge::calculateIndices() {
        indices_.clear();
        const uint sectorMax = static_cast<uint>(ceil(wedgeAngle_ / 360.0f * static_cast<float>(sectorCount_))) + 1;
        for (uint stack = 0; stack < stackCount_; stack++) {
            for (uint sector = 0; sector <= sectorMax; sector++) {
                uint firstIndex = stack * (sectorMax + 1) + sector;
                uint secondIndex = firstIndex + sectorMax + 1;
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
