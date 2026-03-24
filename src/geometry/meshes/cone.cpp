#include <pixelforge/geometry/meshes/cone.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::radians;


    Cone Cone::ConeByRadius(const float height, const float baseRadius, const uint sectorCount) {
        const float halfAngle = atan(baseRadius / height);
        return Cone(height, baseRadius, halfAngle, sectorCount);
    }


    Cone Cone::ConeByAngle(const float height, float halfAngle, const uint sectorCount) {
        if (halfAngle > 85.0f) halfAngle = 85.0f;
        const float radiusBase = height * tan(radians(halfAngle));
        return Cone(height, radiusBase, halfAngle, sectorCount);
    }


    Cone::Cone(const float height, const float baseRadius, const float halfAngle, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->height_ = height;
        this->baseRadius_ = baseRadius;
        this->halfAngle_ = halfAngle;
        this->sectorCount_ = sectorCount;
        generateMeshData();
        Mesh::setupMesh();
    }


    void Cone::calculatePositions() {
        vector<vec3> vertices;
        const float sectorStep = 360.0f / static_cast<float>(sectorCount_);
        for (uint sector = 0; sector <= sectorCount_; sector++) {
            vertices.push_back({vec3(0.0f, 0.0f, 0.0f)});
        }
        for (uint sector = 0; sector <= sectorCount_; sector++) {
            const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
            const float x = baseRadius_ * cos(sectorAngle);
            const float y = baseRadius_ * sin(sectorAngle);
            vertices.push_back({vec3(x, y, 0.0f)});
        }
        for (uint sector = 0; sector <= sectorCount_; sector++) {
            vertices.push_back({vec3(0.0f, 0.0f, height_)});
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void Cone::calculateIndices() {
        indices_.clear();
        for (uint stack = 0; stack < 2; stack++) {
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
