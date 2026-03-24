#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <iostream>


namespace pixelforge::graphics {

    using std::vector;
    using std::map;
    using std::cerr;
    using std::endl;


    void VertexData::setAttribute(const VertexAttribute attribute, const AttributeData& data) {
        GLsizei dataSize;
        visit([&](const auto& attributeData) { dataSize = attributeData.size(); }, data);
        if (attribute == POSITION) {
            vertexCount_ = dataSize;
        } else if (vertexCount_ != dataSize) {
            cerr << "Attribute not added! Data size and Vertex count not matching." << endl;
            return;
        }
        attributes_[attribute] = data;
        updateInterleavedData();
    }


    void VertexData::removeAttribute(const VertexAttribute attribute) {
        attributes_.erase(attribute);
        updateInterleavedData();
    }


    bool VertexData::hasAttribute(const VertexAttribute attribute) const {
        return attributes_.contains(attribute);
    }


    const map<VertexAttribute, VertexData::AttributeData>& VertexData::getAttributes() const {
        return attributes_;
    }


    const std::vector<uint8_t>& VertexData::getInterleavedBuffer() const {
        return interleavedData_;
    }


    const void* VertexData::getInterleavedBufferPointer() const {
        return interleavedData_.empty() ? nullptr : interleavedData_.data();
    }


    GLsizeiptr VertexData::getBufferSize() const {
        return static_cast<GLsizeiptr>(interleavedData_.size());
    }


    GLsizei VertexData::getVertexCount() const {
        return vertexCount_;
    }


    void VertexData::clear() {
        attributes_.clear();
        interleavedData_.clear();
        vertexCount_ = 0;
    }


    void VertexData::updateInterleavedData() const {
        interleavedData_.clear();
        if (vertexCount_ == 0 || attributes_.empty()) return;
        size_t stride = 0;
        map<VertexAttribute, size_t> attributeOffsets;
        for (const auto& [attribute, data] : attributes_) {
            visit([&]<typename T>(const vector<T>&) {
                attributeOffsets[attribute] = stride;
                stride += sizeof(T);
            }, data);
        }
        interleavedData_.resize(vertexCount_ * stride);
        for (GLsizei i = 0; i < vertexCount_; i++) {
            uint8_t* vertexPointer = interleavedData_.data() + i * stride;
            for (const auto& [vertexAttribute, attributeData] : attributes_) {
                visit([&]<typename T>(const vector<T>& data) {
                    const size_t offset = attributeOffsets[vertexAttribute];
                    memcpy(vertexPointer + offset, &data[i], sizeof(T));
                }, attributeData);
            }
        }
    }

} // namespace pixelforge::graphics
