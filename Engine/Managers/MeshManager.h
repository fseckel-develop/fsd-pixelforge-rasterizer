#pragma once
#include "InstanceManager.h"
#include "../Geometry/Meshes/Mesh.h"


/// Extension of the InstanceManager class for managing
/// mesh instances and ensuring their uniqueness.
class MeshManager final : public InstanceManager<Mesh, MeshManager> {
public:
    /// Computes the hash for a mesh instance.
    /// @param mesh The mesh instance to hash.
    /// @return Hash value for the mesh.
    static size_t Hash(const Mesh& mesh) {
        size_t seed = 0;
        for (const auto byte : mesh.GetVertexData().GetInterleavedBuffer()) {
            CombineHashes(seed, std::hash<uint8_t>{}(byte));
        }
        CombineHashes(seed, std::hash<uint>{}(mesh.GetVertexCount()));
        return seed;
    }

    /// Compares two meshes for equality.
    /// @param left The first mesh to compare.
    /// @param right The second mesh to compare.
    /// @return True if the meshes are equal, otherwise false.
    static bool Equals(const Mesh& left, const Mesh& right) {
        const auto& leftData = left.GetVertexData();
        const auto& rightData = right.GetVertexData();
        if (leftData.GetVertexCount() != rightData.GetVertexCount()) return false;
        if (leftData.GetAttributes() != rightData.GetAttributes()) return false;
        if (leftData.GetInterleavedBuffer() != rightData.GetInterleavedBuffer()) return false;
        return true;
    }
};
