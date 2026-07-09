#include "TestingNode.hpp"
#include "SpacialNode.hpp"
#include "SceneManager/SceneCommand/ISceneCommand.hpp"
#include <iostream>
std::unique_ptr<SpacialNode> createNode() {
    return std::make_unique<SpacialNode>();
}
    void spawnerNode::_enterTree(){

    commands_->requestSpawn(this, []() { 
      return std::make_unique<SpacialNode>(); 
    });
      // std::cout<<"hiii"<<std::endl;
    }
