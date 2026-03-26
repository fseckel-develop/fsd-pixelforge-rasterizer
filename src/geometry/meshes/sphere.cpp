#include <pixelforge/geometry/meshes/sphere.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec2;
    using glm::vec3;
    using glm::radians;


    Sphere::Sphere(const float radius, const uint stackCount, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->radius_ = radius;
        this->stackCount_ = stackCount;
        this->sectorCount_ = sectorCount;
        generateMeshData();
    }


    void Sphere::calculatePositions() {
        vector<vec3> positions;
        positions.reserve((stackCount_ + 1) * (sectorCount_ + 1));
        const float sectorStep = radians(360.0f / static_cast<float>(sectorCount_));
        const float stackStep = radians(180.0f / static_cast<float>(stackCount_));
        for (uint stack = 0; stack <= stackCount_; stack++) {
            const float stackAngle = static_cast<float>(stack) * stackStep;
            const float radiusY = radius_ * sin(stackAngle);
            const float y = radius_ * cos(stackAngle);
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                const float sectorAngle = static_cast<float>(sector) * sectorStep;
                const float x = radiusY * sin(sectorAngle);
                const float z = radiusY * cos(sectorAngle);
                positions.push_back({vec3(x, y, z)});
            }
        }
        vertexData_.setAttribute(graphics::POSITION, positions);
    }


    void Sphere::calculateTextureCoordinates() {
        vector<vec2> textureCoordinates;
        textureCoordinates.reserve((stackCount_ + 1) * (sectorCount_ + 1));
        for (uint stack = 0; stack <= stackCount_; stack++) {
            for (uint sector = 0; sector <= sectorCount_; sector++) {
                const float u = static_cast<float>(sector) / static_cast<float>(sectorCount_);
                const float v = static_cast<float>(stack) / static_cast<float>(stackCount_);
                textureCoordinates.push_back({vec2(u, v)});
            }
        }
        vertexData_.setAttribute(graphics::TEXTURE, textureCoordinates);
    }


    void Sphere::calculateNormals() {
        vector<vec3> normals = vertexData_.getAttribute<vec3>(graphics::POSITION);
        for (auto& normal : normals) {
            normal = normalize(normal);
        }
        vertexData_.setAttribute(graphics::NORMAL, normals);
    }


    void Sphere::calculateIndices() {
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
