#pragma once
#include <ostream>
#include <vector>
using namespace std;


class SceneNode : public enable_shared_from_this<SceneNode> {
public:
    explicit SceneNode(const string&);
    void SetName(const string&);
    void AddChild(const shared_ptr<SceneNode>&);
    void RemoveChild(const shared_ptr<SceneNode>&);
    void InsertNodeAbove(const shared_ptr<SceneNode>&);
    [[nodiscard]] int GetID() const;
    [[nodiscard]] string GetName() const;
    [[nodiscard]] shared_ptr<SceneNode> GetParent() const;
    [[nodiscard]] shared_ptr<SceneNode> GetRoot();
    [[nodiscard]] vector<shared_ptr<SceneNode>> GetChildren() const;
    void UpdateSelfAndChildren(float);
    virtual ~SceneNode() = default;

protected:
    int id; string name;
    int ancestorCount = 0;
    weak_ptr<SceneNode> parent;
    vector<shared_ptr<SceneNode>> children;
    virtual void UpdateSelf(float) = 0;

private:
    inline static int nextID = 0;
    virtual void SetParent(const shared_ptr<SceneNode>&);
    bool IsAncestorOf(const shared_ptr<SceneNode>&) const;
};
