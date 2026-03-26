#pragma once
#include <GL/glew.h>
#include "graphics/pipeline/vertex_buffer_layout.hpp"


namespace pixelforge::graphics {

    class VertexData;

    /// Represents a Vertex Buffer Object (VBO) in OpenGL, storing attribute
    /// layout information and managing the upload of vertex data to the GPU.
    /// @note This class follows unique ownership semantics for its underlying OpenGL resource.
    /// Copying is disabled; moving transfers ownership.
    class VertexBuffer {
    public:
        /// Constructs the vertex buffer and uploads the provided vertex data.
        /// @param vertexData The data to be stored in the vertex buffer.
        explicit VertexBuffer(const VertexData& vertexData);

        /// Copy constructor is deleted to prevent copying of OpenGL buffer ownership.
        /// Vertex buffers manage a unique GPU resource and must not be copied.
        VertexBuffer(const VertexBuffer&) = delete;

        /// Copy assignment operator is deleted to prevent copying of OpenGL buffer ownership.
        /// Vertex buffers manage a unique GPU resource and must not be copied.
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        /// Move constructor transfers ownership of the OpenGL buffer from another instance.
        /// @param other The vertex buffer to move from. The source will be left in a valid but empty state.
        VertexBuffer(VertexBuffer&& other) noexcept;

        /// Move assignment operator transfers ownership of the OpenGL buffer from another instance.
        /// @param other The vertex buffer to move from. The source will be left in a valid but empty state.
        /// @return Reference to this instance.
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

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
        void deleteVBO();

        /// Destructor. Automatically deletes the vertex buffer.
        ~VertexBuffer();

    private:
        GLuint vertexBufferID_; ///< OpenGL handle for the vertex buffer object (VBO).
        VertexBufferLayout bufferLayout_; ///< Layout describing the structure of the vertex attributes.
    };

} // namespace pixelforge::graphics
