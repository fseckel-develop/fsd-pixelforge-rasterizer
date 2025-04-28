#pragma once
#include "InstanceManager.h"
#include "../Geometry/Meshes/Mesh.h"
using namespace std;


class MeshManager final : public InstanceManager<Mesh, MeshManager> {
public:
    static size_t Hash(const Mesh& mesh) {
        size_t seed = 0;
        for (const auto byte : mesh.GetVertexData().GetInterleavedVector()) {
            CombineHashes(seed, hash<uint8_t>{}(byte));
        }
        CombineHashes(seed, hash<uint>{}(mesh.GetVertexCount()));
        return seed;
    }

    static bool Equals(const Mesh& left, const Mesh& right) {
        const auto& leftData = left.GetVertexData();
        const auto& rightData = right.GetVertexData();
        if (leftData.GetVertexCount() != rightData.GetVertexCount()) return false;
        if (leftData.GetAttributes() != rightData.GetAttributes()) return false;
        if (leftData.GetInterleavedVector() != rightData.GetInterleavedVector()) return false;
        return true;
    }
};
