#pragma once
#include "nodeSystem/Node.hpp"
#include <memory>
// scene manager holds the true scene structures, alongside flat arrays of nodes for flat traversal where more efficient to do so
class ISceneManager {
  public:
    virtual ~ISceneManager() = default;
    virtual void shutdown() = 0;
    
    virtual void addNodeTree(std::unique_ptr<Node> rootNode, Node* parentInScene = nullptr)=0;
    // node user code execution=
    // FLAT ARRAY
    virtual void update(const double FIXED_DT)=0;


    virtual void flushCommands()=0;


    // calls off to other systems
    virtual void runEngineLogic(const double FIXED_DT)=0;

    // transform calculation math
    // TREE WALK
    virtual void recalculateTransforms()=0;

    //interpolation server utilities: 
    virtual void advanceInterpServer(){}
    

    // node update, user code
    // void runGameLogic(double dt);
    //
    // // backend code, physics, etc
    // void runEngineLogic(double dt);
    //
    // // 
    // void recalculateNodes(double dt);

};

