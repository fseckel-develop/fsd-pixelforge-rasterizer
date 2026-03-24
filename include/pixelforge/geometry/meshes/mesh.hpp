#pragma once
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GL/glew.h>
#include <vector>

namespace pixelforge::graphics { class VertexArray; class VertexData; }

namespace pixelforge::geometry {

    /// Represents a renderable 3D mesh composed of vertex data and OpenGL buffers,
    /// supporting construction from raw vertex data or by parsing OBJ files.
    class Mesh {
    public:
        /// Constructs a mesh from provided vertex data and indices.
        /// @param vertexData The vertex attributes (positions, normals etc.).
        /// @param indices The index buffer data.
        Mesh(graphics::VertexData vertexData, const std::vector<GLuint>& indices);

        /// Constructs a mesh by loading and parsing an OBJ file.
        /// @param fileName Path of the OBJ file relative to the model directory.
        /// @param invertY Whether to invert the Y texture coordinate.
        explicit Mesh(const std::string& fileName, bool invertY = true);

        /// Sets the vertex array for this mesh.
        /// @param vao Shared pointer to a vertex array.
        void setVAO(const std::shared_ptr<graphics::VertexArray>& vao);

        /// Retrieves the vertex array associated with this mesh.
        /// @return Shared pointer to the vertex array.
        [[nodiscard]] const std::shared_ptr<graphics::VertexArray>& getVAO() const;

        /// Retrieves the vertex data of this mesh.
        /// @return The vertex data object.
        [[nodiscard]] const graphics::VertexData& getVertexData() const;

        /// Retrieves the index buffer data of this mesh.
        /// @return The vector of index values.
        [[nodiscard]] const std::vector<GLuint>& getIndices() const;

        /// Retrieves the number of vertices in the mesh.
        /// @return The number of vertices.
        [[nodiscard]] GLsizei getVertexCount() const;

        /// Virtual destructor for cleanup.
        virtual ~Mesh() = default;

    protected:
        std::shared_ptr<graphics::VertexArray> VAO_; ///< Vertex array managing the layout and buffers.
        graphics::VertexData vertexData_; ///< Container for all vertex attributes.
        std::vector<GLuint> indices_; ///< Index buffer data.

        /// Clears and regenerates all vertex attributes and index data.
        void generateMeshData();

        /// Optional override to calculate vertex positions.
        virtual void calculatePositions() {}

        /// Optional override to calculate normals.
        virtual void calculateNormals() {}

        /// Optional override to calculate texture coordinates.
        virtual void calculateTextureCoordinates() {}

        /// Optional override to calculate vertex colors.
        virtual void calculateColors() {}

        /// Optional override to calculate triangle indices.
        virtual void calculateIndices() {}

        /// Sets up the vertex array and GPU buffers based on current vertex data.
        virtual void setupMesh();

        /// Regenerates the mesh data and updates GPU buffers.
        void updateMesh();

    private:
        /// Parses an OBJ file and loads mesh data.
        /// @param fileName Path of the OBJ file relative to the model directory.
        /// @param invertY Whether to invert the Y texture coordinate.
        void parseOBJ(const std::string& fileName, bool invertY);
    };

} // namespace pixelforge::geometry
