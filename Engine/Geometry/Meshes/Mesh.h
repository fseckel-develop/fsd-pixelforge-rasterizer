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
    [[nodiscard]] const VertexArray* GetVAO() const;
    [[nodiscard]] const VertexData& GetVertexData() const;
    [[nodiscard]] const vector<GLuint>& GetIndices() const;
    [[nodiscard]] GLsizei GetVertexCount() const;
    virtual void Render() const;
    virtual ~Mesh();

protected:
    VertexArray* VAO = nullptr;
    VertexData vertexData;
    vector<GLuint> indices;
    GLsizei vertexCount = 0;

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
