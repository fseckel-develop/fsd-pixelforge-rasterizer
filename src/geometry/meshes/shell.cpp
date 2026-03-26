#include <pixelforge/geometry/meshes/shell.hpp>
#include <pixelforge/geometry/utilities.hpp>


namespace pixelforge::geometry {

    using std::vector;
    using glm::vec3;


    Shell::Shell(Curve* outlineCurve, Curve* beltCurve, const uint stackCount, const uint sectorCount):
        Mesh(graphics::VertexData(), vector<GLuint>()) {
        this->outlineCurve_ = outlineCurve;
        this->beltCurve_ = beltCurve;
        this->stackCount_ = stackCount;
        this->sectorCount_ = sectorCount;
        if (outlineCurve && beltCurve) {
            generateMeshData();
        }
        else throw std::invalid_argument("Shell::Shell(): invalid input");
    }


    void Shell::calculatePositions() {
        vector<vec3> vertices;
        const vector<vec3> beltSamples = beltCurve_->sample(sectorCount_);
        const float tStep = (outlineCurve_->getTMax() - outlineCurve_->getTMin()) / static_cast<float>(stackCount_);
        float t = outlineCurve_->getTMin();
        for (uint i = 0; i <= stackCount_; i++) {
            const vec3 outline = outlineCurve_->evaluate(t);
            vector<vec3> belt = utilities::getScaledCopy(beltSamples, vec3(outline.y));
            utilities::translate(belt, vec3(0.0f, 0.0f, outline.x));
            vertices.insert(vertices.end(), belt.begin(), belt.end());
            t += tStep;
        }
        vertexData_.setAttribute(graphics::POSITION, vertices);
    }


    void Shell::calculateIndices() {
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

} // namespace pixelforge::geometry
