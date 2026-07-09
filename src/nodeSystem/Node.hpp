#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include "RenderUtils/RenderTypes.hpp"
#include <print>


// typedef unsigned long long EntityID;
class IRenderServer;
class Node {
  protected:
    // injected
    //children and parent
    std::vector<Node *> children_;
    Node* parent_=nullptr;

  public:

    Node();
    virtual ~Node();
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    // children
    virtual void addChild(Node* child);
    virtual void removeChild(Node* child);
    virtual const std::vector<Node*>& getChildren() const;
    virtual Node* getParent() const; 

    // transformation math
    virtual void checkCalculate(const glm::mat4& parentTransform, bool parentIsDirty){ 
      for (auto *it: children_){
        it->checkCalculate(parentTransform,parentIsDirty);
      }
    };

    // ran on instance 
    virtual void _init(){return;} 

    // called when attached as child 
    virtual void _enterTree(){ return;}
    
    // called before game run loop begins 
    virtual void ready() {return;}

    // game update tick loop
    // USER LOGIC HERE
    virtual void update(double dt);
    
  
    // to call down from engine conveniently
    virtual void _checkCalculate(){
      checkCalculate(glm::mat4(1.0f), false);
    };

    // // 
    // virtual void _movedInTree();
    virtual void printInfo(bool recurse = false, int indent=0);
};
