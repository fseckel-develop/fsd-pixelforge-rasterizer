#pragma once
#include <pixelforge/graphics/pipeline/vertex_buffer_layout.hpp>
#include <GL/glew.h>


namespace pixelforge::graphics {

    class VertexData;

    /// Represents a Vertex Buffer Object (VBO) in OpenGL, storing attribute
    /// layout information and managing the upload of vertex data to the GPU.
    class VertexBuffer {
    public:
        /// Constructs the vertex buffer and uploads the provided vertex data.
        /// @param vertexData The data to be stored in the vertex buffer.
        explicit VertexBuffer(const VertexData& vertexData);

        /// Creates a buffer layout based on the attributes in vertex data.
        /// @param vertexData The vertex data used to determine the layout.
        void createBufferLayout(const VertexData& vertexData);

        /// Binds the vertex buffer to the current OpenGL context.
        void bindVBO() const;

        /// Unbinds the currently bound vertex buffer from the OpenGL context.
        static void unbindVBO();

        /// Retrieves the layout of the vertex buffer.
        /// @return The buffer layout.
        [[nodiscard]] VertexBufferLayout getBufferLayout() const;

        /// Deletes the vertex buffer from GPU memory.
        void deleteVBO() const;

        /// Destructor. Automatically deletes the vertex buffer.
        ~VertexBuffer();

    private:
        GLuint vertexBufferID_; ///< OpenGL handle for the vertex buffer object (VBO).
        VertexBufferLayout bufferLayout_; ///< Layout describing the structure of the vertex attributes.
    };

} // namespace pixelforge::graphics
