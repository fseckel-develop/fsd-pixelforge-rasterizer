#pragma once
#include "Mesh.h"


class Points : public Mesh {
public:
    explicit Points(const vector<vec3>&, float = 1.0f);
    explicit Points(Mesh&, float = 1.0f);
    void Render() const override;

protected:
    float pointSize;
    void SetupMesh() override;
};