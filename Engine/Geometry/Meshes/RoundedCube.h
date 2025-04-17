#pragma once
#include "Shell.h"


class RoundedCube final : public Shell {
public:
    explicit RoundedCube(float = 1.0f, uint = 50);

private:
    float sideLength;
    uint precision;
};
