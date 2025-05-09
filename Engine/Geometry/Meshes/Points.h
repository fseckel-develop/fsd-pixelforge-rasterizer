#pragma once
#include "Mesh.h"


class Points : public Mesh {
public:
    explicit Points(const std::vector<glm::vec3>&, float = 1.0f);
    explicit Points(const Mesh&, float = 1.0f);

protected:
    float pointSize;
    void SetupMesh() override;
};