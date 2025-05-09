#include "Shell.h"
#include "../../Utilities.h"
using namespace std; using namespace glm;


Shell::Shell(Curve* outlineCurve, Curve* beltCurve, const uint stackCount, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->outlineCurve = outlineCurve;
    this->beltCurve = beltCurve;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    if (outlineCurve && beltCurve) {
        GenerateMeshData();
        Mesh::SetupMesh();
    }
}


void Shell::CalculatePositions() {
    vector<vec3> vertices;
    const vector<vec3> beltSamples = beltCurve->Sample(sectorCount);
    const float tStep = (outlineCurve->GetTMax() - outlineCurve->GetTMin()) / static_cast<float>(stackCount);
    float t = outlineCurve->GetTMin();
    for (uint i = 0; i <= stackCount; i++) {
        const vec3 outline = outlineCurve->Evaluate(t);
        vector<vec3> belt = Utilities::ScaledCopy(beltSamples, vec3(outline.y));
        Utilities::Translate(belt, vec3(0.0f, 0.0f, outline.x));
        vertices.insert(vertices.end(), belt.begin(), belt.end());
        t += tStep;
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void Shell::CalculateIndices() {
    indices.clear();
    for (uint stack = 0; stack < stackCount; stack++) {
        for (uint sector = 0; sector < sectorCount; sector++) {
            uint firstIndex = stack * (sectorCount + 1) + sector;
            uint secondIndex = firstIndex + sectorCount + 1;
            // first triangle
            indices.push_back(firstIndex);
            indices.push_back(secondIndex);
            indices.push_back(firstIndex + 1);
            // second triangle
            indices.push_back(secondIndex);
            indices.push_back(secondIndex + 1);
            indices.push_back(firstIndex + 1);
        }
    }
}
