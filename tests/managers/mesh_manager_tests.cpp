#include <doctest.h>
#include <managers/mesh_manager.hpp>
#include <pixelforge/geometry/meshes/mesh.hpp>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GLM/glm.hpp>
#include <memory>
#include <vector>

using pixelforge::geometry::Mesh;
using pixelforge::graphics::NORMAL;
using pixelforge::graphics::POSITION;
using pixelforge::graphics::TEXTURE;
using pixelforge::graphics::VertexData;
using pixelforge::management::MeshManager;


namespace {

    std::shared_ptr<Mesh> makeTriangleMesh(
        const std::vector<glm::vec3>& positions,
        const std::vector<unsigned int>& indices = {0, 1, 2}
    ) {
        VertexData vertexData;
        vertexData.setAttribute(POSITION, positions);
        return std::make_shared<Mesh>(vertexData, indices);
    }

    std::shared_ptr<Mesh> makeTriangleMeshWithNormals(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<unsigned int>& indices = {0, 1, 2}
    ) {
        VertexData vertexData;
        vertexData.setAttribute(POSITION, positions);
        vertexData.setAttribute(NORMAL, normals);
        return std::make_shared<Mesh>(vertexData, indices);
    }

    struct MeshManagerGuard {
        MeshManagerGuard() { MeshManager::clear(); }
        ~MeshManagerGuard() { MeshManager::clear(); }
    };

} // namespace


TEST_CASE("MeshManager returns same pointer for same mesh instance") {
    MeshManagerGuard guard;

    const auto mesh = makeTriangleMesh({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    const auto managedA = MeshManager::getOrCreate(mesh);
    const auto managedB = MeshManager::getOrCreate(mesh);

    CHECK(managedA == mesh);
    CHECK(managedB == mesh);
    CHECK(managedA == managedB);
}

TEST_CASE("MeshManager canonicalizes equivalent meshes with same vertex data") {
    MeshManagerGuard guard;

    const auto meshA = makeTriangleMesh({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    const auto meshB = makeTriangleMesh({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    REQUIRE(meshA != meshB);

    const auto managedA = MeshManager::getOrCreate(meshA);
    const auto managedB = MeshManager::getOrCreate(meshB);

    CHECK(managedA == managedB);
}

TEST_CASE("MeshManager distinguishes meshes with different vertex data") {
    MeshManagerGuard guard;

    const auto meshA = makeTriangleMesh({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    const auto meshB = makeTriangleMesh({
        {0.0f, 0.0f, 0.0f},
        {2.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    const auto managedA = MeshManager::getOrCreate(meshA);
    const auto managedB = MeshManager::getOrCreate(meshB);

    CHECK(managedA != managedB);
}

TEST_CASE("MeshManager distinguishes meshes with different vertex counts") {
    MeshManagerGuard guard;

    VertexData vertexDataA;
    vertexDataA.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    VertexData vertexDataB;
    vertexDataB.setAttribute(POSITION, std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f}
    });

    const auto meshA = std::make_shared<Mesh>(vertexDataA, std::vector<unsigned int>{0, 1, 2});
    const auto meshB = std::make_shared<Mesh>(vertexDataB, std::vector<unsigned int>{0, 1, 2, 1, 3, 2});

    const auto managedA = MeshManager::getOrCreate(meshA);
    const auto managedB = MeshManager::getOrCreate(meshB);

    CHECK(managedA != managedB);
}

TEST_CASE("MeshManager distinguishes meshes with different attribute layouts") {
    MeshManagerGuard guard;

    const auto meshA = makeTriangleMesh({
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    });

    const auto meshB = makeTriangleMeshWithNormals(
        {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}
        },
        {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f}
        }
    );

    const auto managedA = MeshManager::getOrCreate(meshA);
    const auto managedB = MeshManager::getOrCreate(meshB);

    CHECK(managedA != managedB);
}

TEST_CASE("MeshManager distinguishes meshes with different indices") {
    MeshManagerGuard guard;

    const std::vector<glm::vec3> positions = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };

    const auto meshA = makeTriangleMesh(positions, {0, 1, 2});
    const auto meshB = makeTriangleMesh(positions, {2, 1, 0});

    const auto managedA = MeshManager::getOrCreate(meshA);
    const auto managedB = MeshManager::getOrCreate(meshB);

    CHECK(managedA != managedB);
}
