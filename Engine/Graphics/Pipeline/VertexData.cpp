#include "VertexData.h"
#include <iostream>
#include <ostream>


VertexData::VertexData(): vertexCount(0) {}


void VertexData::SetAttribute(const VertexAttribute attribute, const AttributeData& data) {
    GLsizei dataSize;
    visit([&](const auto& attributeData) { dataSize = attributeData.size(); }, data);
    if (attribute == POSITION) {
        vertexCount = dataSize;
    }
    else if (vertexCount != dataSize) {
        cerr << "Attribute not added! Data size and Vertex count not matching." << endl;
        return;
    }
    attributes[attribute] = data;
    UpdateInterleavedData();
}


void VertexData::RemoveAttribute(const VertexAttribute attribute) {
    attributes.erase(attribute);
    UpdateInterleavedData();
}


bool VertexData::HasAttribute(const VertexAttribute attribute) const {
    return attributes.contains(attribute);
}


const map<VertexAttribute, VertexData::AttributeData>& VertexData::GetAttributes() const {
    return attributes;
}


const std::vector<uint8_t>& VertexData::GetInterleavedVector() const {
    return interleavedData;
}


const void* VertexData::GetBufferDataPointer() const {
    return interleavedData.empty() ? nullptr : interleavedData.data();
}


GLsizeiptr VertexData::GetBufferSize() const {
    return static_cast<GLsizeiptr>(interleavedData.size());
}


GLsizei VertexData::GetVertexCount() const {
    return vertexCount;
}


void VertexData::Clear() {
    attributes.clear();
    interleavedData.clear();
    vertexCount = 0;
}


void VertexData::UpdateInterleavedData() const {
    interleavedData.clear();
    if (vertexCount == 0 || attributes.empty()) {
        return;
    }
    size_t stride = 0;
    map<VertexAttribute, size_t> attributeOffsets;
    for (const auto& [attribute, data] : attributes) {
        visit([&]<typename T>(const vector<T>&) {
            attributeOffsets[attribute] = stride;
            stride += sizeof(T);
        }, data);
    }
    interleavedData.resize(vertexCount * stride);
    for (size_t i = 0; i < vertexCount; i++) {
        uint8_t* vertexPointer = interleavedData.data() + i * stride;
        for (const auto& [vertexAttribute, attributeData] : attributes) {
            visit([&]<typename T>(const vector<T>& data) {
                const size_t offset = attributeOffsets[vertexAttribute];
                memcpy(vertexPointer + offset, &data[i], sizeof(T));
            }, attributeData);
        }
    }
}
