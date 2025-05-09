#pragma once
#include "VertexData.h"
#include <vector>
#include <GL/glew.h>
#include <GLM/glm.hpp>


/// Describes a single element in a vertex buffer layout.
typedef struct VertexBufferElement {
    VertexAttribute index; ///< Attribute index (used for shader binding).
    GLint size;  ///< Number of components (e.g. 2 for glm::vec2, 3 for glm::vec3).
    GLenum type; ///< OpenGL data type (e.g. GL_FLOAT).
    GLboolean normalized; ///< Whether the data should be normalized.
    GLsizei offset; ///< Byte offset of the attribute within a vertex.

    /// Returns the size in bytes of the given OpenGL data type.
    /// @param type The OpenGL data type.
    /// @return GLsizei The size in bytes.
    static GLsizei GetTypeSize(const GLenum type) {
        switch (type) {
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
            case GL_FLOAT: return 4;
            default: return 0;
        }
    }
} VertexBufferElement;


/// Describes the layout of attributes in a vertex buffer, which
/// determines how attribute data is interpreted when passed to shaders.
class VertexBufferLayout {
public:
    /// Constructs an empty layout with zero stride.
    VertexBufferLayout();

    /// Adds an attribute element of a given type to the layout.
    /// @tparam DataT The attribute data type (e.g. glm::vec2 or glm::vec3).
    /// @param index The attribute index to be used for shader binding.
    template <typename DataT>
    void AddElement(VertexAttribute index);

    /// Removes an attribute element from the layout by index.
    /// @param index The attribute index to remove.
    void RemoveElement(VertexAttribute index);

    /// Returns the list of all attribute elements in the layout.
    /// @return Vector of layout elements.
    [[nodiscard]] const std::vector<VertexBufferElement>& GetElements() const;

    /// Returns the total stride (in bytes) of one vertex data package.
    /// @return Total stride in bytes.
    [[nodiscard]] GLsizei GetStride() const;

private:
    std::vector<VertexBufferElement> elements; ///< List of elements describing the layout.
    GLsizei stride; ///< Byte size of a single vertex.
    GLuint nextAvailableIndex; ///< Internal tracker for assigning new indices.

    /// Recalculates offsets and total stride after layout changes.
    void UpdateLayout();
};


/// Adds a vec2 attribute element to the layout.
/// @param index The attribute index.
template <>
inline void VertexBufferLayout::AddElement<glm::vec2>(const VertexAttribute index) {
    const VertexBufferElement newElement = {index, 2, GL_FLOAT, GL_FALSE, stride};
    elements.push_back(newElement);
    stride += VertexBufferElement::GetTypeSize(GL_FLOAT) * 2;
}


/// Adds a vec3 attribute element to the layout.
/// @param index The attribute index.
template <>
inline void VertexBufferLayout::AddElement<glm::vec3>(const VertexAttribute index) {
    const VertexBufferElement newElement = {index, 3, GL_FLOAT, GL_FALSE, stride};
    elements.push_back(newElement);
    stride += VertexBufferElement::GetTypeSize(GL_FLOAT) * 3;
}
