#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>
#include "managers/instance_manager.hpp"


namespace pixelforge::management {

    /// Extension of the InstanceManager class for managing
    /// mesh instances and ensuring their uniqueness.
    class MeshManager final : public InstanceManager<geometry::Mesh, MeshManager> {
    public:
        /// Computes the hash for a mesh instance.
        /// @param mesh The mesh instance to hash.
        /// @return Hash value for the mesh.
        static size_t hash(const geometry::Mesh& mesh) {
            size_t seed = 0;
            for (const auto byte : mesh.getVertexData().getInterleavedBuffer()) {
                combineHashes(seed, std::hash<uint8_t>{}(byte));
            }
            combineHashes(seed, std::hash<GLsizei>{}(mesh.getVertexCount()));
            for (const auto index : mesh.getIndices()) {
                combineHashes(seed, std::hash<GLuint>{}(index));
            }
            return seed;
        }

        /// Compares two meshes for equality.
        /// @param left The first mesh to compare.
        /// @param right The second mesh to compare.
        /// @return True if the meshes are equal, otherwise false.
        static bool equals(const geometry::Mesh& left, const geometry::Mesh& right) {
            const auto& leftData = left.getVertexData();
            const auto& rightData = right.getVertexData();
            if (leftData.getVertexCount() != rightData.getVertexCount()) return false;
            if (leftData.getAttributes() != rightData.getAttributes()) return false;
            if (leftData.getInterleavedBuffer() != rightData.getInterleavedBuffer()) return false;
            if (left.getIndices() != right.getIndices()) return false;
            return true;
        }
    };

} // namespace pixelforge::management
