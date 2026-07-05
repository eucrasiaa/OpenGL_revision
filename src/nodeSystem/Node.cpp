#include "Node.hpp"
#include <algorithm> 
#include <stdexcept>

Node::Node(IRenderServer* renderServer) : renderServer_(renderServer) {
  if (!renderServer) {
    throw std::runtime_error("RenderServer cannot be null");
  }
}
Node::~Node() {
  for (auto* child : children_) {
    delete child;
  }
  children_.clear();
}
void Node::addChild(Node* child) {
  if (child) {
    child->parent_ = this;
    children_.push_back(child);
  }
}


void Node::removeChild(Node* child) {
  auto it = std::find(children_.begin(), children_.end(), child);
  if (it != children_.end()) {
    (*it)->parent_ = nullptr;
    children_.erase(it);
  }
}

void Node::update(double dt) {
  for (auto *elem : children_) {
    elem->update(dt);
  }
}

Node* Node::getParent() const { return parent_; }

const std::vector<Node*>& Node::getChildren() const { return children_; }



void Node::printInfo(bool recurse, int indent){
  std::print("{:{}} Node:\n","",indent);
  std::print("{:{}} Children: {}\n","",indent, children_.size());
  std::print("{:{}}Parent: {:#x}\n", "", indent, 
      (parent_ != nullptr) ? reinterpret_cast<std::uintptr_t>(parent_) : 0);
  std::print("{:{}}--------------------\n", "", indent);
  indent=indent+5;
  

  if(recurse){
    for (auto *it : children_){
      it->printInfo(true,indent);      
    }
  }
}
