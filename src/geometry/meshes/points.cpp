#include <pixelforge/geometry/meshes/points.hpp>
#include "graphics/pipeline/vertex_array.hpp"
#include "graphics/pipeline/vertex_buffer.hpp"


namespace pixelforge::geometry {

    using graphics::VertexBuffer;
    using std::vector;
    using std::make_shared;
    using glm::vec3;


    Points::Points(const vector<vec3>& points, const float pointSize):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->vertexData_.clear();
        vertexData_.setAttribute(graphics::POSITION, points);
        this->pointSize_ = pointSize;
        Points::setupMesh();
    }


    Points::Points(const Mesh& mesh, const float pointSize):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->vertexData_ = mesh.getVertexData();
        this->pointSize_ = pointSize;
        Points::setupMesh();
    }


    void Points::setupMesh() {
        if (!vertexData_.hasAttribute(graphics::POSITION)) return;
        VAO_ = make_shared<graphics::VertexArray>();
        const auto VBO = VertexBuffer(vertexData_);
        VAO_->addVertexBuffer(VBO);
    }

    // TODO: Refactor this into the Renderer class
    // void Points::Render() const {
    //     VAO->BindVAO();
    //     glPointSize(pointSize);
    //     glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertexData.GetAttribute<vec3>(POSITION).size()));
    //     glPointSize(1.0f);
    //     VertexArray::UnbindVAO();
    // }

} // namespace pixelforge::geometry
