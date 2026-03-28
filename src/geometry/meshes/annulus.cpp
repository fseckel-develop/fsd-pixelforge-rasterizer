#include <pixelforge/geometry/meshes/annulus.hpp>
#include <pixelforge/graphics/pipeline/vertex_data.hpp>
#include <GLM/glm.hpp>
#include <numbers>
#include <stdexcept>
#include <vector>


namespace pixelforge::geometry {

    using glm::vec2;
    using glm::vec3;
    using std::numbers::pi_v;
    using std::vector;

    namespace {

        constexpr unsigned int VERTICES_PER_RING = 2; // inner + outer
        constexpr unsigned int SIDES = 2;             // top + bottom

    } // namespace

    Annulus::Annulus(const float innerRadius, const float outerRadius, const unsigned int segmentCount) :
        Mesh(graphics::VertexData{}, {}),
        innerRadius_(innerRadius),
        outerRadius_(outerRadius),
        segmentCount_(segmentCount) {
        if (innerRadius_ < 0.0f) {
            throw std::invalid_argument("Annulus inner radius must be non-negative.");
        }
        if (outerRadius_ <= innerRadius_) {
            throw std::invalid_argument("Annulus outer radius must be greater than inner radius.");
        }
        if (segmentCount_ < 3) {
            throw std::invalid_argument("Annulus segment count must be at least 3.");
        }
        generateMeshData();
    }


    void Annulus::calculatePositions() {
        vector<vec3> positions;
        positions.reserve(static_cast<size_t>(SIDES) * (segmentCount_ + 1u) * VERTICES_PER_RING);
        const float step = 2.0f * pi_v<float> / static_cast<float>(segmentCount_);
        // Top side (+Y normal)
        for (unsigned int i = 0; i <= segmentCount_; i++) {
            const float angle = static_cast<float>(i) * step;
            const float c = std::cos(angle);
            const float s = std::sin(angle);

            positions.emplace_back(innerRadius_ * c, 0.0f, innerRadius_ * s);
            positions.emplace_back(outerRadius_ * c, 0.0f, outerRadius_ * s);
        }
        // Bottom side (-Y normal)
        for (unsigned int i = 0; i <= segmentCount_; i++) {
            const float angle = static_cast<float>(i) * step;
            const float c = std::cos(angle);
            const float s = std::sin(angle);

            positions.emplace_back(innerRadius_ * c, 0.0f, innerRadius_ * s);
            positions.emplace_back(outerRadius_ * c, 0.0f, outerRadius_ * s);
        }
        vertexData_.setAttribute(graphics::POSITION, positions);
    }


    void Annulus::calculateNormals() {
        vector<vec3> normals;
        normals.reserve(static_cast<size_t>(SIDES) * (segmentCount_ + 1u) * VERTICES_PER_RING);
        // Top side normals
        for (unsigned int i = 0; i <= segmentCount_; i++) {
            normals.emplace_back(0.0f, 1.0f, 0.0f);
            normals.emplace_back(0.0f, 1.0f, 0.0f);
        }
        // Bottom side normals
        for (unsigned int i = 0; i <= segmentCount_; i++) {
            normals.emplace_back(0.0f, -1.0f, 0.0f);
            normals.emplace_back(0.0f, -1.0f, 0.0f);
        }
        vertexData_.setAttribute(graphics::NORMAL, normals);
    }


    void Annulus::calculateTextureCoordinates() {
        std::vector<vec2> textureCoordinates;
        textureCoordinates.reserve(static_cast<size_t>(2) * (segmentCount_ + 1u) * 2u);
        // Top side
        for (unsigned int i = 0; i <= segmentCount_; i++) {
            const float v = static_cast<float>(i) / static_cast<float>(segmentCount_);
            textureCoordinates.emplace_back(0.0f, v); // inner
            textureCoordinates.emplace_back(1.0f, v); // outer
        }
        // Bottom side
        for (unsigned int i = 0; i <= segmentCount_; i++) {
            const float v = static_cast<float>(i) / static_cast<float>(segmentCount_);
            textureCoordinates.emplace_back(0.0f, v); // inner
            textureCoordinates.emplace_back(1.0f, v); // outer
        }
        vertexData_.setAttribute(graphics::TEXTURE, textureCoordinates);
    }


    void Annulus::calculateIndices() {
        indices_.clear();
        indices_.reserve(static_cast<size_t>(SIDES) * segmentCount_ * 6u);
        const unsigned int sideVertexCount = (segmentCount_ + 1u) * VERTICES_PER_RING;
        const unsigned int bottomOffset = sideVertexCount;
        // Top side: CCW when viewed from +Y
        for (unsigned int i = 0; i < segmentCount_; i++) {
            constexpr unsigned int topOffset = 0u;
            const unsigned int base = topOffset + i * VERTICES_PER_RING;
            const GLuint inner0 = base;
            const GLuint outer0 = base + 1u;
            const GLuint inner1 = base + 2u;
            const GLuint outer1 = base + 3u;
            indices_.push_back(inner0);
            indices_.push_back(outer0);
            indices_.push_back(outer1);
            indices_.push_back(inner0);
            indices_.push_back(outer1);
            indices_.push_back(inner1);
        }

        // Bottom side: reverse winding so it is visible from below
        for (unsigned int i = 0; i < segmentCount_; i++) {
            const unsigned int base = bottomOffset + i * VERTICES_PER_RING;
            const GLuint inner0 = base;
            const GLuint outer0 = base + 1u;
            const GLuint inner1 = base + 2u;
            const GLuint outer1 = base + 3u;
            indices_.push_back(inner0);
            indices_.push_back(outer1);
            indices_.push_back(outer0);
            indices_.push_back(inner0);
            indices_.push_back(inner1);
            indices_.push_back(outer1);
        }
    }

} // namespace pixelforge::geometry
