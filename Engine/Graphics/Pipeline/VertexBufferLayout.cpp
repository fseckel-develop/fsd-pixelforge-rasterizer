#include "VertexBufferLayout.h"
using namespace std;


VertexBufferLayout::VertexBufferLayout():
    stride(0),
    nextAvailableIndex(0) {
}


void VertexBufferLayout::RemoveElement(const VertexAttribute index) {
    const auto it = ranges::remove_if(
        elements, [&](const VertexBufferElement& element) -> bool {
            return element.index == index;
        }).begin();
    if (it != elements.end()) {
        elements.erase(it, elements.end());
        UpdateLayout();
    }
}


const vector<VertexBufferElement>& VertexBufferLayout::GetElements() const {
    return elements;
}


GLsizei VertexBufferLayout::GetStride() const {
    return stride;
}


void VertexBufferLayout::UpdateLayout() {
    stride = 0;
    for (auto& element : elements) { // NOLINT
        element.offset = stride;
        stride += VertexBufferElement::GetTypeSize(element.type) * element.size;
    }
}
