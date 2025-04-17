#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include <map>
using namespace std; using namespace glm;


typedef enum VertexAttribute {
    POSITION = 0,
    TEXTURE = 1,
    NORMAL = 2,
    COLOR = 3
} VertexAttribute;


class VertexData {
public:
    using AttributeData = variant<vector<vec2>, vector<vec3>>;

    VertexData();
    void SetAttribute(VertexAttribute, const AttributeData&);
    void RemoveAttribute(VertexAttribute);
    const map<VertexAttribute, AttributeData>& GetAttributes() const;
    template <typename T> const vector<T>& GetAttribute(VertexAttribute) const;
    [[nodiscard]] bool HasAttribute(VertexAttribute) const;
    [[nodiscard]] const void* GetBufferData() const;
    [[nodiscard]] GLsizeiptr GetBufferSize() const;
    void Clear();

private:
    map<VertexAttribute, AttributeData> attributes;
    mutable vector<uint8_t> interleavedData;
    unsigned int vertexCount;
    void UpdateInterleavedData() const;
};


template <typename T>
const vector<T>& VertexData::GetAttribute(const VertexAttribute attribute) const {
    if (const auto it = attributes.find(attribute); it != attributes.end()) {
        return get<vector<T>>(it->second);
    }
    static const vector<T> empty;
    return empty;
}
