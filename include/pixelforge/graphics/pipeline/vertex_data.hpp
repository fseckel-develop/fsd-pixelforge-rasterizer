#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <map>


namespace pixelforge::graphics {

    /// Enumerates standard vertex attribute types, with integer assignment for GLSL usage.
    enum VertexAttribute {
        POSITION = 0, ///< Vertex position attribute
        TEXTURE = 1,  ///< Texture coordinate attribute
        NORMAL = 2,   ///< Normal vector attribute
        COLOR = 3     ///< Vertex color attribute
    };


    /// Allows storing and managing vertex attributes, such as position, normal
    /// and texture coordinates, and provides a GPU-friendly interleaved format.
    class VertexData {
    public:
        /// Variant type for supported attribute data (vector of glm::vec2 or glm::vec3).
        using AttributeData = std::variant<std::vector<glm::vec2>, std::vector<glm::vec3>>;

        /// Sets or updates an attribute in the vertex data.
        /// @param attribute The type of vertex attribute.
        /// @param data The attribute data (glm::vec2 or glm::vec3).
        void setAttribute(VertexAttribute attribute, const AttributeData& data);

        /// Removes an attribute from the vertex data.
        /// @param attribute The attribute to remove.
        void removeAttribute(VertexAttribute attribute);

        /// Returns all attributes and their data.
        /// @return Reference to the internal map of attributes.
        const std::map<VertexAttribute, AttributeData>& getAttributes() const;

        /// Returns a specific attribute vector by type.
        /// @tparam AttributeT glm::vec2 or glm::vec3.
        /// @param attribute The attribute type to retrieve.
        /// @return The attribute vector or an empty one if not present.
        template <typename AttributeT>
        const std::vector<AttributeT>& getAttribute(const VertexAttribute attribute) const {
            if (const auto it = attributes_.find(attribute); it != attributes_.end()) {
                return get<std::vector<AttributeT>>(it->second);
            }
            static const std::vector<AttributeT> empty;
            return empty;
        }

        /// Checks whether a specific attribute exists in the vertex dataset.
        /// @param attribute The attribute to check.
        /// @return True if present, false otherwise.
        [[nodiscard]] bool hasAttribute(VertexAttribute attribute) const;

        /// Returns the raw interleaved byte buffer for OpenGL.
        /// @return Reference to the interleaved byte buffer.
        [[nodiscard]] const std::vector<uint8_t>& getInterleavedBuffer() const;

        /// Returns a pointer to the raw interleaved byte buffer.
        /// @return Pointer to the interleaved byte buffer.
        [[nodiscard]] const void* getInterleavedBufferPointer() const;

        /// Returns the size of the buffer in bytes needed for the vertex dataset.
        /// @return Buffer size in bytes.
        [[nodiscard]] GLsizeiptr getBufferSize() const;

        /// Returns the number of vertices in the dataset.
        /// @return Vertex count.
        [[nodiscard]] GLsizei getVertexCount() const;

        /// Clears all vertex data and attributes.
        void clear();

    private:
        std::map<VertexAttribute, AttributeData> attributes_; ///< Attribute data mapped by type.
        mutable std::vector<uint8_t> interleavedData_; ///< Interleaved byte buffer for OpenGL.
        GLsizei vertexCount_ = 0; ///< Number of vertices in the dataset.

        /// Updates the interleaved vertex buffer from current attributes.
        /// Called automatically after changes to the attribute data.
        void updateInterleavedData() const;
    };

} // namespace pixelforge::graphics
