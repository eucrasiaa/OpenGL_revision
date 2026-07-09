#pragma once
#include "ISceneManager.hpp"
#include "SceneManager/SceneCommand/ISceneCommand.hpp"
#include "nodeSystem/Node.hpp"
#include <memory>


class SceneManager : public ISceneManager, public ISceneCommand {
  protected:
    std::vector<std::unique_ptr<Node>> masterList_;

    // only holds elements that are childed from spacial?
    std::vector<Node*> iterList_;


    // cleared when flushed
    std::vector<SpawnAction> nodesToSpawn_;
    // std::vector<Node*> nodesToSpawn;
    std::vector<Node*> nodesToDestroy_;

  public:
    // node user code execution
    // FLAT ARRAY
    virtual void update(const double FIXED_DT) override;


    virtual void flushCommands() override;


    // calls off to other systems
    virtual void runEngineLogic(const double FIXED_DT) override;

    // transform calculation math
    // TREE WALK
    virtual void recalculateTransforms() override;


    virtual void requestSpawn(Node* parent, std::function<std::unique_ptr<Node>()> factory) override;

    virtual void requestDestroy(Node* node) override;



};
