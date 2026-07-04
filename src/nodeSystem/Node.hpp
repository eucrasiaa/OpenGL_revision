#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
class IRenderServer;

class Node {
  protected:
    // injected
    IRenderServer* renderServer_ = nullptr;  
    //children and parent
    std::vector<Node *> children_;
    Node* parent_=nullptr;
  public:

    Node(IRenderServer* renderServer);
    virtual ~Node();
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    // children
    void addChild(Node* child);
    void removeChild(Node* child);
    const std::vector<Node*>& getChildren() const;
    Node* getParent() const; 

    // game logic


    virtual void checkCalculate(const glm::mat4& parentTransform, bool parentIsDirty){ 
      for (auto *it: children_){
        it->checkCalculate(parentTransform,parentIsDirty);
      }
    };


    // ran on instance 
    void _init(); 

    // called when attached as child 
    void _enterTree();
    
    // called before game run loop begins 
    void ready();

    // game update tick loop
    void update(double dt);

    // update caches post update + update render server
    void checkCalculate(){
      checkCalculate(glm::mat4(1.0f), false);
    };

    // // 
    // virtual void _movedInTree();
    virtual void printInfo(bool recurse = false, int indent=0);

    // virtual ~Node();
    // // virtual void render(const glm::mat4 &parentTransform);
    // virtual void addChild(Node *node);
    //
    // virtual void render(const glm::mat4& parentGlobal, bool parentIsDirty);
    // void triggerCompute();
    // virtual void computeTransforms(const glm::mat4& parentGlobal, bool parentIsDirty);
};
