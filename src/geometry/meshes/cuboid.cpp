#include <pixelforge/geometry/meshes/cuboid.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;


    Cuboid::Cuboid(const float width, const float height, const float depth):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->width_ = width;
        this->height_ = height;
        this->depth_ = depth;
        generateMeshData();
    }


    void Cuboid::calculatePositions() {
        std::vector<vec3> vertices;
        const float x = width_ / 2.0f;
        const float y = height_ / 2.0f;
        const float z = depth_ / 2.0f;
        vertices.push_back({vec3(-x, -y, -z)});
        vertices.push_back({vec3(-x, -y, +z)});
        vertices.push_back({vec3(-x, +y, -z)});
        vertices.push_back({vec3(-x, +y, +z)});
        vertices.push_back({vec3(+x, -y, -z)});
        vertices.push_back({vec3(+x, -y, +z)});
        vertices.push_back({vec3(+x, +y, -z)});
        vertices.push_back({vec3(+x, +y, +z)});
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void Cuboid::calculateIndices() {
        indices_.clear();
        indices_ = {
            0, 1, 2, 1, 3, 2,
            0, 5, 1, 0, 4, 5,
            1, 5, 3, 3, 5, 7,
            2, 3, 7, 2, 7, 6,
            4, 6, 7, 4, 7, 5,
            0, 2, 6, 0, 6, 4
        };
    }

} // namespace pixelforge::geometry
