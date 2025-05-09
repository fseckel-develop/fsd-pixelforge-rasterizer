#pragma once
#include "../../Graphics/Pipeline/VertexData.h"
#include <GL/glew.h>
#include <vector>
class VertexArray;


/// Represents a renderable 3D mesh composed of vertex data and OpenGL buffers,
/// supporting construction from raw vertex data or by parsing OBJ files.
class Mesh {
public:
    /// Constructs a mesh from provided vertex data and indices.
    /// @param vertexData The vertex attributes (positions, normals etc.).
    /// @param indices The index buffer data.
    Mesh(VertexData vertexData, const std::vector<GLuint>& indices);

    /// Constructs a mesh by loading and parsing an OBJ file.
    /// @param fileName Path of the OBJ file relative to the model directory.
    /// @param invertY Whether to invert the Y texture coordinate.
    explicit Mesh(const std::string& fileName, bool invertY = true);

    /// Sets the vertex array for this mesh.
    /// @param vao Shared pointer to a vertex array.
    void SetVAO(const std::shared_ptr<VertexArray>& vao);

    /// Retrieves the vertex array associated with this mesh.
    /// @return Shared pointer to the vertex array.
    [[nodiscard]] const std::shared_ptr<VertexArray>& GetVAO() const;

    /// Retrieves the vertex data of this mesh.
    /// @return The vertex data object.
    [[nodiscard]] const VertexData& GetVertexData() const;

    /// Retrieves the index buffer data of this mesh.
    /// @return The vector of index values.
    [[nodiscard]] const std::vector<GLuint>& GetIndices() const;

    /// Retrieves the number of vertices in the mesh.
    /// @return The number of vertices.
    [[nodiscard]] GLsizei GetVertexCount() const;

    /// Virtual destructor for cleanup.
    virtual ~Mesh() = default;

protected:
    std::shared_ptr<VertexArray> VAO; ///< Vertex array managing the layout and buffers.
    VertexData vertexData;            ///< Container for all vertex attributes.
    std::vector<GLuint> indices;      ///< Index buffer data.

    /// Clears and regenerates all vertex attributes and index data.
    void GenerateMeshData();

    /// Optional override to calculate vertex positions.
    virtual void CalculatePositions() {}

    /// Optional override to calculate normals.
    virtual void CalculateNormals() {}

    /// Optional override to calculate texture coordinates.
    virtual void CalculateTextureCoordinates() {}

    /// Optional override to calculate vertex colors.
    virtual void CalculateColors() {}

    /// Optional override to calculate triangle indices.
    virtual void CalculateIndices() {}

    /// Sets up the vertex array and GPU buffers based on current vertex data.
    virtual void SetupMesh();

    /// Regenerates the mesh data and updates GPU buffers.
    void UpdateMesh();

private:
    /// Parses an OBJ file and loads mesh data.
    /// @param fileName Path of the OBJ file relative to the model directory.
    /// @param invertY Whether to invert the Y texture coordinate.
    void ParseOBJ(const std::string& fileName, bool invertY);
};
