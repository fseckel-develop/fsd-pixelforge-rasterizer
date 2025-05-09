#pragma once
#include <GL/glew.h>
#include <vector>


/// Represents an Index Buffer Object (IBO) in
/// OpenGL, managing rendering indexed geometry.
class IndexBuffer {
public:
    /// Constructs an index buffer and uploads the provided index data.
    /// @param data A vector of indices to initialize the buffer with.
    explicit IndexBuffer(const std::vector<GLuint>& data);

    /// Binds the index buffer to the current OpenGL context.
    void BindIBO() const;

    /// Unbinds the currently bound index buffer from the OpenGL context.
    static void UnbindIBO();

    /// Deletes the index buffer from GPU memory.
    void DeleteIBO() const;

    /// Destructor. Automatically deletes the index buffer.
    ~IndexBuffer();

private:
    GLuint indexBufferID; ///< OpenGL handle for the index buffer object (IBO).
};
