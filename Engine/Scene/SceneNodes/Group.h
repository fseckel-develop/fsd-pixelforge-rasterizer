#pragma once
#include "SceneNode.h"


class Group : public SceneNode {
public:
    explicit Group(const string&);
    void UpdateSelf(float) override {}
};
