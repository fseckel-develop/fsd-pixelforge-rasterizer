#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <map>


/// Enumerates standard vertex attribute types, with integer assignment for GLSL usage.
typedef enum VertexAttribute {
    POSITION = 0, ///< Vertex position attribute
    TEXTURE = 1,  ///< Texture coordinate attribute
    NORMAL = 2,   ///< Normal vector attribute
    COLOR = 3     ///< Vertex color attribute
} VertexAttribute;


/// Allows storing and managing vertex attributes, such as position, normal
/// and texture coordinates, and provides a GPU-friendly interleaved format.
class VertexData {
public:
    /// Variant type for supported attribute data (vector of glm::vec2 or glm::vec3).
    using AttributeData = std::variant<std::vector<glm::vec2>, std::vector<glm::vec3>>;

    /// Sets or updates an attribute in the vertex data.
    /// @param attribute The type of vertex attribute.
    /// @param data The attribute data (glm::vec2 or glm::vec3).
    void SetAttribute(VertexAttribute attribute, const AttributeData& data);

    /// Removes an attribute from the vertex data.
    /// @param attribute The attribute to remove.
    void RemoveAttribute(VertexAttribute attribute);

    /// Returns all attributes and their data.
    /// @return Reference to the internal map of attributes.
    const std::map<VertexAttribute, AttributeData>& GetAttributes() const;

    /// Returns a specific attribute vector by type.
    /// @tparam AttributeT glm::vec2 or glm::vec3.
    /// @param attribute The attribute type to retrieve.
    /// @return The attribute vector or an empty one if not present.
    template <typename AttributeT>
    const std::vector<AttributeT>& GetAttribute(const VertexAttribute attribute) const {
        if (const auto it = attributes.find(attribute); it != attributes.end()) {
            return get<std::vector<AttributeT>>(it->second);
        }
        static const std::vector<AttributeT> empty;
        return empty;
    }

    /// Checks whether a specific attribute exists in the vertex dataset.
    /// @param attribute The attribute to check.
    /// @return True if present, false otherwise.
    [[nodiscard]] bool HasAttribute(VertexAttribute attribute) const;

    /// Returns the raw interleaved byte buffer for OpenGL.
    /// @return Reference to the interleaved byte buffer.
    [[nodiscard]] const std::vector<uint8_t>& GetInterleavedBuffer() const;

    /// Returns a pointer to the raw interleaved byte buffer.
    /// @return Pointer to the interleaved byte buffer.
    [[nodiscard]] const void* GetInterleavedBufferPointer() const;

    /// Returns the size of the buffer in bytes needed for the vertex dataset.
    /// @return Buffer size in bytes.
    [[nodiscard]] GLsizeiptr GetBufferSize() const;

    /// Returns the number of vertices in the dataset.
    /// @return Vertex count.
    [[nodiscard]] GLsizei GetVertexCount() const;

    /// Clears all vertex data and attributes.
    void Clear();

private:
    std::map<VertexAttribute, AttributeData> attributes; ///< Attribute data mapped by type.
    mutable std::vector<uint8_t> interleavedData; ///< Interleaved byte buffer for OpenGL.
    GLsizei vertexCount = 0; ///< Number of vertices in the dataset.

    /// Updates the interleaved vertex buffer from current attributes.
    /// Called automatically after changes to the attribute data.
    void UpdateInterleavedData() const;
};
