#include <pixelforge/geometry/meshes/cylinder.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::radians;


    Cylinder::Cylinder(const float radius, const float height, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->radius_ = radius;
        this->height_ = height;
        this->sectorCount_ = sectorCount;
        generateMeshData();
    }


    void Cylinder::calculatePositions() {
        vector<vec3> vertices;
        const float sectorStep = 360.0f / static_cast<float>(sectorCount_);
        for (uint sector = 0; sector <= sectorCount_; sector++) {
            vertices.push_back({vec3(0.0f, 0.0f, 0.0f)});
        }
        for (uint sector = 0; sector <= sectorCount_; sector++) {
            const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
            const float x = radius_ * cos(sectorAngle);
            const float y = radius_ * sin(sectorAngle);
            vertices.push_back({vec3(x, y, 0.0f)});
        }
        for (uint sector = 0; sector <= sectorCount_; sector++) {
            const float sectorAngle = radians(static_cast<float>(sector) * sectorStep);
            const float x = radius_ * cos(sectorAngle);
            const float y = radius_ * sin(sectorAngle);
            vertices.push_back({vec3(x, y, height_)});
        }
        for (uint sector = 0; sector <= sectorCount_; sector++) {
            vertices.push_back({vec3(0.0f, 0.0f, height_)});
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void Cylinder::calculateIndices() {
        indices_.clear();
        for (uint stack = 0; stack < 3; stack++) {
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
