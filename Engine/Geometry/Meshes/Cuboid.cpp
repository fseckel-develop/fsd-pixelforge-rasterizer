#include "Cuboid.h"
using namespace std; using namespace glm;


Cuboid::Cuboid(const float width, const float height, const float depth):
    Mesh(VertexData(), vector<GLuint>()) {
    this->width = width;
    this->height = height;
    this->depth = depth;
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Cuboid::CalculatePositions() {
    std::vector<vec3> vertices;
    const float x = width / 2.0f;
    const float y = height / 2.0f;
    const float z = depth / 2.0f;
    vertices.push_back({vec3(-x, -y, -z)});
    vertices.push_back({vec3(-x, -y, +z)});
    vertices.push_back({vec3(-x, +y, -z)});
    vertices.push_back({vec3(-x, +y, +z)});
    vertices.push_back({vec3(+x, -y, -z)});
    vertices.push_back({vec3(+x, -y, +z)});
    vertices.push_back({vec3(+x, +y, -z)});
    vertices.push_back({vec3(+x, +y, +z)});
    vertexData.SetAttribute(POSITION, vertices);
}


void Cuboid::CalculateIndices() {
    indices.clear();
    indices = {
        0, 1, 2, 1, 3, 2,
        0, 5, 1, 0, 4, 5,
        1, 5, 3, 3, 5, 7,
        2, 3, 7, 2, 7, 6,
        4, 6, 7, 4, 7, 5,
        0, 2, 6, 0, 6, 4
    };
}
