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
    void SetNodeScale(float);
    void MarkGlobalTransformDirty();
    virtual void AddAnimation(const shared_ptr<Animation>&);
    [[nodiscard]] bool IsGlobalTransformDirty() const;
    [[nodiscard]] const Transform& GetGlobalTransform() const;
    [[nodiscard]] virtual mat4 GetModelMatrix() const;

protected:
    Scale nodeScale;
    Transform localTransform;
    mutable Transform globalTransform;
    mutable bool globalTransformDirty = true;
    void UpdateSelf(float) override;
    static shared_ptr<TransformNode> FindNextTransformAncestorFrom(const shared_ptr<SceneNode>&);

private:
    void UpdateGlobalTransform() const;
};
