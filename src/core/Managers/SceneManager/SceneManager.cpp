#include "SceneManager.hpp"

void SceneManager::update(double dt){
  for (Node* it : iterList_){
    it->update(dt);
  }
}

void SceneManager::runEngineLogic(double dt){
  return;
}

// must be tree walk
void SceneManager::recalculateTransforms(){
  for (auto* node : rootNodes_) {
    if (node != nullptr) { 
      node->_checkCalculate();
    }
  } 
}


void SceneManager::registerBranchElements(Node* node) {
  if (!node) return;
  node->_init(this); 
  iterList_.push_back(node);


  for (Node* child : node->getChildren()) {
    registerBranchElements(child);
  }
}
// void SceneManager::flushJustPending(){
//
// }

void SceneManager::addNodeTree(std::unique_ptr<Node> rootNode, Node* parentInScene){
  pendingBranches_.push_back({ std::move(rootNode), parentInScene });
} 

void SceneManager::requestSpawn(Node* parent, std::function<std::unique_ptr<Node>()> factory) {
  nodesToSpawn_.push_back({parent, factory});
}

void SceneManager::requestSpawn(Node* parent, std::function<Node*()> rawFactory) {
    auto wrappedFactory = [rawFactory = std::move(rawFactory)]() -> std::unique_ptr<Node> {
        Node* rawNode = rawFactory();
        return std::unique_ptr<Node>(rawNode);
    };
    requestSpawn(parent, std::move(wrappedFactory));
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
        ptr->onDelete();
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
    // hand off command!! 
    rawPtr->_init(this); 

    //list 2
    if (action.parent) {
      action.parent->addChild(rawPtr);
    }
    //list 1
    iterList_.push_back(rawPtr);
    // handoff ownership
    masterList_.push_back(std::move(newNode));
  }
  nodesToSpawn_.clear();


  for (auto& branch : pendingBranches_) {
    Node* branchRootRaw = branch.root.get();

    // Setup tree hierarchy attachment
    if (branch.parent) {
      branch.parent->addChild(branchRootRaw);
    } 
    else {
      //is a root, so add to roots! 
      rootNodes_.push_back(branchRootRaw); 
      branchRootRaw->_enterTree();
    }
    //recursively walk
    registerBranchElements(branchRootRaw);

    masterList_.push_back(std::move(branch.root));
  }
}


void SceneManager::shutdown(){
  for (Node* node : iterList_) {
    if (node) {
      node->onDelete(); 
    }
  }
  iterList_.clear();
  rootNodes_.clear();
  nodesToDestroy_.clear();

  nodesToSpawn_.clear(); 
  pendingBranches_.clear();
  // as unique ptrs theyll kill down
  masterList_.clear();
}
