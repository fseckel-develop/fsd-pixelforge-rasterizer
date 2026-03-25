#include <pixelforge/geometry/meshes/tube.hpp>
#include <pixelforge/geometry/utilities.hpp>
#include <Eigen/Dense>
#include <GLM/gtc/quaternion.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;
    using glm::quat;
    using glm::mat3;


    Tube::Tube(Curve* spineCurve, Curve* beltCurve, const uint stackCount, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->spineCurve_ = spineCurve;
        this->beltCurve_ = beltCurve;
        this->stackCount_ = stackCount;
        this->sectorCount_ = sectorCount;
        computeBeltBase();
        generateMeshData();
        Mesh::setupMesh();
    }


    void Tube::calculatePositions() {
        vector<vec3> vertices;
        vector<vec3> beltSamples = beltCurve_->sample(sectorCount_, Curve::POSITION);
        utilities::translate(beltSamples, -beltCurve_->getCentroid());
        const vector<quat> beltRotations = computeBeltRotations();
        vector<vec3> firstBelt;
        const float tStep = (spineCurve_->getTMax() - spineCurve_->getTMin()) / static_cast<float>(stackCount_);
        float t = spineCurve_->getTMin();
        for (uint i = 0; i <= stackCount_; i++) {
            const vec3 spinePoint = spineCurve_->evaluate(t);
            vector<vec3> belt = utilities::getRotatedCopy(beltSamples, beltRotations[i]);
            utilities::translate(belt, spinePoint);
            vertices.insert(vertices.end(), belt.begin(), belt.end());
            t += tStep;
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void Tube::calculateIndices() {
        indices_.clear();
        for (uint stack = 0; stack < stackCount_; stack++) {
            for (uint sector = 0; sector < sectorCount_; sector++) {
                uint firstIndex = stack * (sectorCount_ + 1) + sector;
                uint secondIndex = firstIndex + sectorCount_ + 1;
                // first triangle
                indices_.push_back(firstIndex);
                indices_.push_back(secondIndex);
                indices_.push_back(firstIndex + 1);
                // second triangle
                indices_.push_back(secondIndex);
                indices_.push_back(secondIndex + 1);
                indices_.push_back(firstIndex + 1);
            }
        }
    }


    void Tube::computeBeltBase() {
        beltBase_[0] = computeBeltNormal();
        beltBase_[1] = computeBeltRadiant(beltBase_[0]);
        beltBase_[2] = normalize(cross(beltBase_[0], beltBase_[1]));
    }


    vec3 Tube::computeBeltNormal() const {
        const vector<vec3> samples = beltCurve_->sample(sectorCount_);
        if (samples.size() < 3) {
            return {0.0f, 0.0f, 1.0f};
        }
        const vec3 centroid = beltCurve_->getCentroid();
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


    vec3 Tube::computeBeltRadiant(const vec3 normal) const {
        const vec3 radiant = beltCurve_->evaluate(beltCurve_->getTMin()) - beltCurve_->getCentroid();
        return normalize(cross(cross(normal, radiant), normal));
    }


    vector<quat> Tube::computeBeltRotations() const {
        vector<mat3> frames;
        const float tStep = (spineCurve_->getTMax() - spineCurve_->getTMin()) / static_cast<float>(stackCount_);
        float t = spineCurve_->getTMin();
        for (uint i = 0; i <= stackCount_; i++) {
            const mat3 frame = spineCurve_->computeRotMinFrame(t);
            frames.emplace_back(frame);
            t += tStep;
        }
        vector<quat> beltRotations;
        for (const auto& frame : frames) {
            quat beltRotation = glm::normalize(glm::quat_cast(frame * transpose(beltBase_)));
            beltRotations.emplace_back(beltRotation);
        }
        return beltRotations;
    }

} // namespace pixelforge::geometry
