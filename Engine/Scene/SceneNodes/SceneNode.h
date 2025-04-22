#pragma once
#include "../Transforms/Transform.h"
#include "../Transforms/Scale.h"
#include <GLM/glm.hpp>
#include <vector>
class Animation;
using namespace std; using namespace glm;


class SceneNode : public enable_shared_from_this<SceneNode> {
public:
    explicit SceneNode(const string&);
    void SetName(const string&);
    void SetParent(const shared_ptr<SceneNode>&);
    void SetTransform(const Transform&);
    void SetNodeScale(const vec3&);
    void AddAnimation(Animation*);
    [[nodiscard]] string GetName() const;
    [[nodiscard]] mat4 GetModelMatrix() const;
    virtual void Update(float);
    virtual ~SceneNode() = default;

protected:
    int id; string name;
    weak_ptr<SceneNode> parent;
    vector<shared_ptr<SceneNode>> children;
    int ancestorCount = 0;
    Transform initialTransform;
    Transform localTransform;
    Scale scale;
    vector<shared_ptr<Animation>> animations;
    mutable mat4 modelMatrix = mat4(1.0f);
    mutable bool transformDirty = true;

private:
    inline static int nextID = 0;
    [[nodiscard]] string GenerateUniqueName() const;
    void AddChild(const shared_ptr<SceneNode>&);
    void RemoveChild(const shared_ptr<SceneNode>&);
    void MarkTransformDirty();
};
