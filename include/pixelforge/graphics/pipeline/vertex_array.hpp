#pragma once
#include <GL/glew.h>
#include <vector>


namespace pixelforge::graphics {

    class VertexBuffer; class IndexBuffer;

    /// Represents a Vertex Array Object (VAO) in OpenGL, managing vertex
    /// and index buffers and handles binding and setup of vertex attributes.
    class VertexArray {
    public:
        /// Constructs a new vertex array.
        VertexArray();

        /// Binds the vertex array to the current OpenGL context.
        void bindVAO() const;

        /// Unbinds the currently bound vertex array from the OpenGL context.
        static void unbindVAO();

        /// Adds a vertex buffer to the vertex array and sets up vertex attribute pointers.
        /// @param vertexBuffer The vertex buffer to add.
        void addVertexBuffer(const VertexBuffer& vertexBuffer);

        /// Sets the index buffer for the vertex array.
        /// @param indexBuffer The index buffer to set.
        void setIndexBuffer(const IndexBuffer& indexBuffer);

        /// Gets the OpenGL ID of the vertex array.
        /// @return The vertex array ID.
        [[nodiscard]] GLuint getID() const;

        /// Gets the list of vertex buffers associated with the vertex array.
        /// @return List of vertex buffers.
        [[nodiscard]] const std::vector<VertexBuffer>& getVertexBuffers() const;

        /// Gets the index buffer currently set for the vertex array.
        /// @return Pointer to the index buffer.
        [[nodiscard]] const IndexBuffer* getIndexBuffer() const;

        /// Deletes the vertex array and associated buffers from GPU memory.
        void deleteVAO() const;

        /// Destructor. Automatically deletes the vertex array and associated buffers.
        ~VertexArray();

    private:
        GLuint vertexArrayID_; ///< OpenGL handle for the vertex array object (VAO).
        std::vector<VertexBuffer> vertexBuffers_; ///< List of vertex buffers attached to the vertex array.
        const IndexBuffer* indexBuffer_; ///< Pointer to the index buffer.
    };

} // namespace pixelforge::graphics
