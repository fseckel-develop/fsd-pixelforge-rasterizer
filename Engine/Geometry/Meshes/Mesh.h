#pragma once
#include "../../Graphics/Pipeline/VertexData.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
class VertexArray;
using namespace std;
using namespace glm;


class Mesh {
public:
    Mesh(VertexData, const vector<GLuint>&);
    explicit Mesh(const string&, bool = true);
    const VertexData& GetVertexData();
    const vector<GLuint>& GetIndices();
    virtual void Render() const;
    virtual ~Mesh();

protected:
    VertexData vertexData;
    vector<GLuint> indices;
    GLsizei vertexCount = 0;
    VertexArray* VAO = nullptr;

    void GenerateMeshData();
    virtual void CalculatePositions() {}
    virtual void CalculateNormals() {}
    virtual void CalculateTextureCoordinates() {}
    virtual void CalculateColors() {}
    virtual void CalculateIndices() {}
    virtual void SetupMesh();
    void UpdateMesh();

private:
    void ParseOBJ(const string&, bool);
};
