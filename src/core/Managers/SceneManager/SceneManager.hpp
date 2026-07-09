#pragma once
#include "ISceneManager.hpp"
#include "SceneManager/SceneCommand/ISceneCommand.hpp"
#include "nodeSystem/Node.hpp"
#include <memory>


struct PendingBranch {
    std::unique_ptr<Node> root;
    Node* parent;
};

class SceneManager : public ISceneManager, public ISceneCommand {
  protected:
    std::vector<std::unique_ptr<Node>> masterList_;


    // to be slotted in 
    std::vector<PendingBranch> pendingBranches_;
    // only holds elements that are childed from spacial?
    std::vector<Node*> iterList_;


    // cleared when flushed
    std::vector<SpawnAction> nodesToSpawn_;
    // std::vector<Node*> nodesToSpawn;
    std::vector<Node*> nodesToDestroy_;


    std::vector<Node*> rootNodes_;
    // part of debug
    // void flushJustPending();

    void registerBranchElements(Node* node);
  public:
    virtual void shutdown() override;
    // for now, placeholder basically 
    virtual void addNodeTree(std::unique_ptr<Node> rootNode, Node* parentInScene = nullptr) override;

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



    virtual void requestSpawn(Node* parent, Node* rawNode) override;
    virtual void requestDestroy(Node* node) override;



};
