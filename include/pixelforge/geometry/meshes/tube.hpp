#pragma once
#include <pixelforge/geometry/meshes/mesh.hpp>
#include <pixelforge/geometry/curves/curve.hpp>


namespace pixelforge::geometry {

    class Tube final : public Mesh {
    public:
        explicit Tube(Curve*, Curve*, uint = 50, uint = 50);

    private:
        Curve* spineCurve_;
        Curve* beltCurve_;
        glm::mat3 beltBase_ = glm::mat3();
        uint stackCount_;
        uint sectorCount_;
        void calculatePositions() override;
        void calculateIndices() override;
        void computeBeltBase();
        [[nodiscard]] glm::vec3 computeBeltNormal() const;
        [[nodiscard]] glm::vec3 computeBeltRadiant(glm::vec3) const;
        [[nodiscard]] std::vector<glm::quat> computeBeltRotations() const;
    };

} // namespace pixelforge::geometry
