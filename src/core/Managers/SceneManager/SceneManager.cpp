#include "SceneManager.hpp"

void SceneManager::requestSpawn(Node* parent, std::function<std::unique_ptr<Node>()> factory) {
  nodesToSpawn_.push_back({parent, factory});
}

void SceneManager::requestDestroy(Node* node) {
  // check double queueing?
  if (std::find(nodesToDestroy_.begin(), nodesToDestroy_.end(), node) == nodesToDestroy_.end()) {
    nodesToDestroy_.push_back(node);
  }
}

void SceneManager::flushCommands() {
  // delete first
  for (Node* nodeToDelete : nodesToDestroy_) {
    // pop from optimized iter list
    auto itIter = std::find(iterList_.begin(), iterList_.end(), nodeToDelete);
    if (itIter != iterList_.end()) {
      *itIter = iterList_.back();
      iterList_.pop_back();
    }
    // remove tie to parent
    if (nodeToDelete->getParent()) {
      nodeToDelete->getParent()->removeChild(nodeToDelete);
    }

    // remove from master list
    auto itMaster = std::find_if(masterList_.begin(), masterList_.end(), 
        [nodeToDelete](const std::unique_ptr<Node>& ptr) {
        return ptr.get() == nodeToDelete;
        });

    if (itMaster != masterList_.end()) {
      masterList_.erase(itMaster); // to trigger the unique_ptr thing, no delete/free
    }
  }
  nodesToDestroy_.clear();


  // commands_.requestSpawn(this, []() {
  //     auto bullet = std::make_unique<BulletNode>();
  //     return bullet;
  // });
  
  // THEN do spawns
  for (auto& action : nodesToSpawn_) {
    // run the lambda?
    std::unique_ptr<Node> newNode = action.factory();
    Node* rawPtr = newNode.get();

    // Link the tree hierarchy
    if (action.parent) {
      action.parent->addChild(rawPtr);
    }

    // Push to flat list for fast loops
    iterList_.push_back(rawPtr);

    // Relinquish unique ownership to the master lifetime manager
    masterList_.push_back(std::move(newNode));
  }
  nodesToSpawn_.clear();
}
