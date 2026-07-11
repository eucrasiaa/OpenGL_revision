#include "TestingNode.hpp"
#include "SpacialNode.hpp"
#include "Scene.hpp"
#include <glm/ext/vector_float3.hpp>
#include <iostream>
    void spawnerNode::_enterTree(){

      Scene::spawn(this, []() { 
      return std::make_unique<spawnerNode>(); 
    });

    }


void spawnerNode::printInfo(bool recurse, int indent) {
  std::cout<<"HIIIIII\n\n\n\n\n"<<std::endl;
  SpacialNode::printInfo(recurse,indent);
}

void spawnerNode::update(double dt){
  setPosition(glm::vec3(position_.x+1,position_.y,position_.z));
}
