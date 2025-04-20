#include "Points.h"
#include "../../Graphics/Pipeline/VertexArray.h"
#include "../../Graphics/Pipeline/VertexBuffer.h"


Points::Points(const vector<vec3>& points, const float pointSize):
    Mesh(VertexData(), vector<GLuint>()) {
    this->vertexData.Clear();
    vertexData.SetAttribute(POSITION, points);
    this->pointSize = pointSize;
    Points::SetupMesh();
}


Points::Points(const Mesh& mesh, const float pointSize):
    Mesh(VertexData(), vector<GLuint>()) {
    this->vertexData = mesh.GetVertexData();
    this->pointSize = pointSize;
    Points::SetupMesh();
}


void Points::SetupMesh() {
    if (!vertexData.HasAttribute(POSITION)) return;
    VAO = new VertexArray();
    const auto VBO = VertexBuffer(vertexData);
    VAO->AddVertexBuffer(VBO);
}


void Points::Render() const {
    VAO->BindVAO();
    glPointSize(pointSize);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertexData.GetAttribute<vec3>(POSITION).size()));
    glPointSize(1.0f);
    VertexArray::UnbindVAO();
}
