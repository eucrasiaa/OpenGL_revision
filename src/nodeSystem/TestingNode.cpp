#include "TestingNode.hpp"
#include "SpacialNode.hpp"
#include "Scene.hpp"
    void spawnerNode::_enterTree(){

      Scene::spawn(this, []() { 
      return std::make_unique<SpacialNode>(); 
    });
      std::cout<<"hiii"<<std::endl;

    }


void spawnerNode::printInfo(bool recurse, int indent) {
  std::cout<<"HIIIIII\n\n\n\n\n"<<std::endl;
  SpacialNode::printInfo(recurse,indent);
} 
