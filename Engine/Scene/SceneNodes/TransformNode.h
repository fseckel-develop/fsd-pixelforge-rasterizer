#pragma once
#include "SceneNode.h"
#include "../Transforms/Transform.h"
#include "../Transforms/Scale.h"
class Animation;
using namespace std; using namespace glm;


class TransformNode : public SceneNode {
public:
    explicit TransformNode(const string&);
    void SetTransform(const Transform&);
    void SetNodeScale(const vec3&);
    void MarkGlobalTransformDirty();
    void AddAnimation(Animation*);
    [[nodiscard]] bool IsGlobalTransformDirty() const;
    [[nodiscard]] const Transform& GetGlobalTransform() const;
    [[nodiscard]] mat4 GetModelMatrix() const;

protected:
    Transform localTransform;
    mutable Transform globalTransform;
    mutable bool globalTransformDirty = true;
    void UpdateSelf(float) override;
    static shared_ptr<TransformNode> FindNextTransformAncestorFrom(const shared_ptr<SceneNode>&);

private:
    Scale nodeScale;
    void UpdateGlobalTransform() const;
};
