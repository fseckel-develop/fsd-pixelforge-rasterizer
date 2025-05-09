#include "Tube.h"
#include <Eigen/Dense>
#include <GLM/gtc/quaternion.hpp>
#include "../../Utilities.h"
using namespace std; using namespace glm;


Tube::Tube(Curve* spineCurve, Curve* beltCurve, const uint stackCount, const uint sectorCount):
    Mesh(VertexData(), vector<GLuint>()) {
    this->spineCurve = spineCurve;
    this->beltCurve = beltCurve;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    ComputeBeltBase();
    GenerateMeshData();
    Mesh::SetupMesh();
}


void Tube::CalculatePositions() {
    vector<vec3> vertices;
    vector<vec3> beltSamples = beltCurve->Sample(sectorCount, Curve::POSITION);
    Utilities::Translate(beltSamples, -beltCurve->GetCentroid());
    const vector<quat> beltRotations = ComputeBeltRotations();
    vector<vec3> firstBelt;
    const float tStep = (spineCurve->GetTMax() - spineCurve->GetTMin()) / static_cast<float>(stackCount);
    float t = spineCurve->GetTMin();
    for (uint i = 0; i <= stackCount; i++) {
        const vec3 spinePoint = spineCurve->Evaluate(t);
        vector<vec3> belt = Utilities::RotatedCopy(beltSamples, beltRotations[i]);
        Utilities::Translate(belt, spinePoint);
        vertices.insert(vertices.end(), belt.begin(), belt.end());
        t += tStep;
    }
    vertexData.SetAttribute(POSITION, vertices);
}


void Tube::CalculateIndices() {
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


void Tube::ComputeBeltBase() {
    beltBase[0] = ComputeBeltNormal();
    beltBase[1] = ComputeBeltRadiant(beltBase[0]);
    beltBase[2] = normalize(cross(beltBase[0], beltBase[1]));
}


vec3 Tube::ComputeBeltNormal() const {
    const vector<vec3> samples = beltCurve->Sample(sectorCount);
    if (samples.size() < 3) {
        return {0.0f, 0.0f, 1.0f};
    }
    const vec3 centroid = beltCurve->GetCentroid();
    mat3 covariance(0.0f);
    for (const vec3& sample : samples) {
        vec3 d = sample - centroid;
        covariance += outerProduct(d, d);
    }
    Eigen::Matrix3f covarianceMatrix;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            covarianceMatrix(i, j) = covariance[i][j];
        }
    }
    const Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> solver(covarianceMatrix);
    int minIndex;
    solver.eigenvalues().minCoeff(&minIndex);
    Eigen::Vector3f normalEigenVector = solver.eigenvectors().col(minIndex);
    return normalize(vec3(normalEigenVector.x(), normalEigenVector.y(), normalEigenVector.z()));
}


vec3 Tube::ComputeBeltRadiant(const vec3 normal) const {
    const vec3 radiant = beltCurve->Evaluate(beltCurve->GetTMin()) - beltCurve->GetCentroid();
    return normalize(cross(cross(normal, radiant), normal));
}


vector<quat> Tube::ComputeBeltRotations() const {
    vector<mat3> frames;
    const float tStep = (spineCurve->GetTMax() - spineCurve->GetTMin()) / static_cast<float>(stackCount);
    float t = spineCurve->GetTMin();
    for (uint i = 0; i <= stackCount; i++) {
        const mat3 frame = spineCurve->ComputeRotMinFrame(t);
        frames.emplace_back(frame);
        t += tStep;
    }
    vector<quat> beltRotations;
    for (const auto& frame : frames) {
        quat beltRotation = normalize(quat_cast(frame * transpose(beltBase)));
        beltRotations.emplace_back(beltRotation);
    }
    return beltRotations;
}
