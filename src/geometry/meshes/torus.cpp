#include <pixelforge/geometry/meshes/torus.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::radians;


    Torus::Torus(const float innerRadius, const float torusRadius, const uint stackCount, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->innerRadius_ = innerRadius;
        this->torusRadius_ = torusRadius;
        this->stackCount_ = stackCount;
        this->sectorCount_ = sectorCount;
        generateMeshData();
    }


    void Torus::calculatePositions() {
        vector<vec3> vertices;
        const float sectorStep = radians(360.0f / static_cast<float>(sectorCount_));
        const float stackStep = radians(360.0f / static_cast<float>(stackCount_));
        for (uint stack = 0; stack <= stackCount_; stack++) {
            const float stackAngle = static_cast<float>(stack) * stackStep;
            const float dx = cos(stackAngle) * torusRadius_;
            const float z = sin(stackAngle) * torusRadius_;
            const float radiusZ = innerRadius_ + torusRadius_ - dx;
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                const float sectorAngle = static_cast<float>(sector) * sectorStep;
                const float x = radiusZ * cos(sectorAngle);
                const float y = radiusZ * sin(sectorAngle);
                vertices.push_back({vec3(x, y, z)});
            }
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void Torus::calculateIndices() {
        indices_.clear();
        for (uint stack = 0; stack < stackCount_; stack++) {
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
