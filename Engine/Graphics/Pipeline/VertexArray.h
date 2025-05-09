#pragma once
#include <GL/glew.h>
#include <vector>
class VertexBuffer; class IndexBuffer;


/// Represents a Vertex Array Object (VAO) in OpenGL, managing vertex
/// and index buffers and handles binding and setup of vertex attributes.
class VertexArray {
public:
    /// Constructs a new vertex array.
    VertexArray();

    /// Binds the vertex array to the current OpenGL context.
    void BindVAO() const;

    /// Unbinds the currently bound vertex array from the OpenGL context.
    static void UnbindVAO();

    /// Adds a vertex buffer to the vertex array and sets up vertex attribute pointers.
    /// @param vertexBuffer The vertex buffer to add.
    void AddVertexBuffer(const VertexBuffer& vertexBuffer);

    /// Sets the index buffer for the vertex array.
    /// @param indexBuffer The index buffer to set.
    void SetIndexBuffer(const IndexBuffer& indexBuffer);

    /// Gets the OpenGL ID of the vertex array.
    /// @return The vertex array ID.
    [[nodiscard]] GLuint GetID() const;

    /// Gets the list of vertex buffers associated with the vertex array.
    /// @return List of vertex buffers.
    [[nodiscard]] const std::vector<VertexBuffer>& GetVertexBuffers() const;

    /// Gets the index buffer currently set for the vertex array.
    /// @return Pointer to the index buffer.
    [[nodiscard]] const IndexBuffer* GetIndexBuffer() const;

    /// Deletes the vertex array and associated buffers from GPU memory.
    void DeleteVAO() const;

    /// Destructor. Automatically deletes the vertex array and associated buffers.
    ~VertexArray();

private:
    GLuint vertexArrayID; ///< OpenGL handle for the vertex array object (VAO).
    std::vector<VertexBuffer> vertexBuffers; ///< List of vertex buffers attached to the vertex array.
    const IndexBuffer* indexBuffer; ///< Pointer to the index buffer.
};
