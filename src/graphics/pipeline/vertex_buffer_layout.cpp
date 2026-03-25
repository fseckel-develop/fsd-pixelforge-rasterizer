#include "graphics/pipeline/vertex_buffer_layout.hpp"


namespace pixelforge::graphics {

    using std::vector;
    using std::ranges::remove_if;


    VertexBufferLayout::VertexBufferLayout():
        stride_(0),
        nextAvailableIndex(0) {
    }


    void VertexBufferLayout::removeElement(const VertexAttribute index) {
        const auto it = remove_if(
            elements_, [&](const VertexBufferElement& element) -> bool {
                return element.index == index;
            }).begin();
        if (it != elements_.end()) {
            elements_.erase(it, elements_.end());
            UpdateLayout_();
        }
    }


    const vector<VertexBufferElement>& VertexBufferLayout::getElements() const {
        return elements_;
    }


    GLsizei VertexBufferLayout::getStride() const {
        return stride_;
    }


    void VertexBufferLayout::UpdateLayout_() {
        stride_ = 0;
        for (auto& element : elements_) { // NOLINT
            element.offset = stride_;
            stride_ += VertexBufferElement::getTypeSize(element.type) * element.size;
        }
    }

} // namespace pixelforge::graphics
