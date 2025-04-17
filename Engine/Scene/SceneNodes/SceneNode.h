#pragma once
#include "../Transforms/Transform.h"
#include <GLM/glm.hpp>
#include <vector>
class Animation;
using namespace std; using namespace glm;


class SceneNode {
public:
    explicit SceneNode(const string&);
    void SetName(const string&);
    void SetTransform(const Transform&);
    void AddAnimation(Animation*);
    void AddChild(SceneNode*);
    void RemoveChild(SceneNode*);
    [[nodiscard]] int GetID() const;
    [[nodiscard]] string GetName() const;
    [[nodiscard]] const SceneNode* GetRoot() const;
    virtual void Update(float);
    virtual ~SceneNode() = default;

protected:
    int id;
    string name;
    Transform initialTransform;
    Transform localTransform;
    vector<Animation*> animations;
    SceneNode* parent = nullptr;
    vector<SceneNode*> children;
    int ancestorCount = 0;

    void SetParent(SceneNode*);
    [[nodiscard]] virtual mat4 GetModelMatrix() const;

private:
    inline static int nextID = 0;
    [[nodiscard]] string GenerateUniqueName() const;
};
