#include "Polygon.h"
#include "../../Graphics/Pipeline/VertexArray.h"


Polygon::Polygon(const vector<vec3>& points):
    Points(points) {
}


Polygon::Polygon(const Curve& curve, const uint sampleCount, const Curve::EvaluationType type):
    Points(vector<vec3>()) {
    vertexData.Clear();
    vertexData.SetAttribute(POSITION, curve.Sample(sampleCount, type));
    Points::SetupMesh();
}


void Polygon::Render() const {
    VAO->BindVAO();
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertexData.GetAttribute<vec3>(POSITION).size()));
    VertexArray::UnbindVAO();
}
