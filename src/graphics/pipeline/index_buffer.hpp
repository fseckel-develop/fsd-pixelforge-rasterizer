#pragma once
#include <GL/glew.h>
#include <vector>


namespace pixelforge::graphics {

    /// Represents an Index Buffer Object (IBO) in
    /// OpenGL, managing rendering indexed geometry.
    /// @note This class follows unique ownership semantics for its underlying OpenGL resource.
    /// Copying is disabled; moving transfers ownership.
    class IndexBuffer {
    public:
        /// Constructs an index buffer and uploads the provided index data.
        /// @param data A vector of indices to initialize the buffer with.
        explicit IndexBuffer(const std::vector<GLuint>& data);

        /// Copy constructor is deleted to prevent copying of OpenGL buffer ownership.
        /// Index buffers manage a unique GPU resource and must not be copied.
        IndexBuffer(const IndexBuffer&) = delete;

        /// Copy assignment operator is deleted to prevent copying of OpenGL buffer ownership.
        /// Index buffers manage a unique GPU resource and must not be copied.
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        /// Move constructor transfers ownership of the OpenGL buffer from another instance.
        /// @param other The index buffer to move from. The source will be left in a valid but empty state.
        IndexBuffer(IndexBuffer&& other) noexcept;

        /// Move assignment operator transfers ownership of the OpenGL buffer from another instance.
        /// @param other The index buffer to move from. The source will be left in a valid but empty state.
        /// @return Reference to this instance.
        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        /// Binds the index buffer to the current OpenGL context.
        void bindIBO() const;

        /// Unbinds the currently bound index buffer from the OpenGL context.
        static void unbindIBO();

        /// Deletes the index buffer from GPU memory.
        void deleteIBO();

        /// Destructor. Automatically deletes the index buffer.
        ~IndexBuffer();

    private:
        GLuint indexBufferID_; ///< OpenGL handle for the index buffer object (IBO).
    };

} // namespace::graphics
