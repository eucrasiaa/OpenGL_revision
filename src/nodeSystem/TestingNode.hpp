#pragma once
#include "SpacialNode.hpp"
#include "Managers/SceneManager/SceneCommand/ISceneCommand.hpp"

class spawnerNode : public SpacialNode{

  public:
    using SpacialNode::SpacialNode;
    virtual void _enterTree() override;
    virtual void printInfo(bool recurse, int indent) override;
virtual void update(double dt) override;

};
