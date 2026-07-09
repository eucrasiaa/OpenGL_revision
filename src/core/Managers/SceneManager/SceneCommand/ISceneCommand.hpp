#pragma once
#include "nodeSystem/Node.hpp"
#include <memory>

struct SpawnAction {
    Node* parent;
    std::function<std::unique_ptr<Node>()> factory;
};

class ISceneCommand {
  public: 
    virtual ~ISceneCommand() = default;
    virtual void requestSpawn(Node* parent, std::function<std::unique_ptr<Node>()> factory) = 0;
  virtual void requestDestroy(Node* node) = 0;
};
