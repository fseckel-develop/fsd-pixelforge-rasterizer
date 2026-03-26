#include <pixelforge/geometry/meshes/polygon.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;


    Polygon::Polygon(const vector<vec3>& points):
        Points(points) {
    }


    Polygon::Polygon(const Curve& curve, const uint sampleCount, const Curve::EvaluationType type):
        Points(vector<vec3>()) {
        vertexData_.clear();
        vertexData_.setAttribute(graphics::POSITION, curve.sample(sampleCount, type));
    }


    // TODO: Refactor this into the Renderer class
    // void Polygon::Render() const {
    //     VAO->BindVAO();
    //     glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertexData.GetAttribute<vec3>(POSITION).size()));
    //     VertexArray::UnbindVAO();
    // }

} // namespace pixelforge::geometry
