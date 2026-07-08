#include "SpacialNode.hpp"
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>

void SpacialNode::checkCalculate(const glm::mat4& parentTransform, bool parentIsDirty) {
  // did OUR global change?
  bool globalChanged = false;
  //check ourselves first
  if (localDirty_){
    recalculateLocalTransform();
    // local change forces global change
    globalChanged = true;
  }

  // recalculate global if Parent or We moved
  if (parentIsDirty || globalChanged) {
    computeTransforms(parentTransform);
    globalChanged = true;
  }

  // traverse down
  for (auto* child : children_) {
    child->checkCalculate(globalTransform_, globalChanged);
  }

}

// only time localDirty_ should be reset
void SpacialNode::recalculateLocalTransform(){
  // first convert quaentern ? to 4x4 rot 
  glm::mat4 rotMatrix = glm::mat4_cast(quaternion_);

  // local transform = Translate * Rotate * Scale 
  localTransform_ = glm::translate(glm::mat4(1.0f), position_) 
    * rotMatrix 
    * glm::scale(glm::mat4(1.0f), scale_);
  localDirty_=false;
}

void SpacialNode::computeTransforms(const glm::mat4& parentTransform) {

  //global = parent * local 
  globalTransform_ = parentTransform * localTransform_;

  //update basis 
  right_ =   glm::normalize(glm::vec3(globalTransform_[0]));
  up_ =      glm::normalize(glm::vec3(globalTransform_[1]));
  // consider it might be backwasrds ?
  forward_ = glm::normalize(glm::vec3(globalTransform_[2]));
}


















// this is run... when?
// no check so trust user ig
void SpacialNode::setLocalTransform(const glm::mat4& transform) {
  // if ever needed, you gotta pull out the P/R/S vals as they are the source of truth

  position_ = glm::vec3(transform[3]);
  scale_.x = glm::length(glm::vec3(transform[0]));
  scale_.y = glm::length(glm::vec3(transform[1]));
  scale_.z = glm::length(glm::vec3(transform[2]));
  glm::mat3 rotMat(
    glm::vec3(transform[0]) / scale_.x,
    glm::vec3(transform[1]) / scale_.y,
    glm::vec3(transform[2]) / scale_.z
  );
  quaternion_ = glm::quat_cast(rotMat);
  localDirty_ = true;
}
void SpacialNode::setPosition(const glm::vec3& position) {
  if (position_ != position){
    position_ = position;
    localDirty_ = true;
  }
  // localTransform_[3] = glm::vec4(position, 1.0f);
}
void SpacialNode::setRotationQ(const glm::quat& quaternion) {
  // quat oddness, q and -q are identical?
  if (quaternion_ != quaternion){
    quaternion_ = quaternion; 
    localDirty_ = true;
  }
}
void SpacialNode::setScale(const glm::vec3& scale) {
  if (scale_ != scale) {
    scale_ = scale;
    localDirty_ = true;
  }
}
void SpacialNode::markDirty() {
  localDirty_ = true;
}
void SpacialNode::printInfo(bool recurse, int indent) {
  std::print("{:{}}Spacial Node:\n", "", indent);
  std::print("{:{}}  Children: {}\n", "", indent, children_.size());
  std::print("{:{}}  Parent: {:#x}\n", "", indent, 
      (parent_ != nullptr) ? reinterpret_cast<std::uintptr_t>(parent_) : 0);
  std::print("{:{}}  Dirty State: {}\n", "", indent, localDirty_ ? "true" : "false");

  std::print("{:{}}  Rotation (Quat): [w: {:.2f}, x: {:.2f}, y: {:.2f}, z: {:.2f}]\n", 
      "", indent, quaternion_.w, quaternion_.x, quaternion_.y, quaternion_.z);

  std::print("{:{}}  Basis Vectors:\n", "", indent);
  std::print("{:{}}    Forward: [{:.2f}, {:.2f}, {:.2f}]\n", "", indent, forward_.x, forward_.y, forward_.z);
  std::print("{:{}}    Up:      [{:.2f}, {:.2f}, {:.2f}]\n", "", indent, up_.x, up_.y, up_.z);
  std::print("{:{}}    Right:   [{:.2f}, {:.2f}, {:.2f}]\n", "", indent, right_.x, right_.y, right_.z);

  std::print("{:{}}  Local Transform Matrix:\n", "", indent);
  for (int row = 0; row < 4; ++row) {
    std::print("{:{}}    [{:5.2f}, {:5.2f}, {:5.2f}, {:5.2f}]\n", "", indent,
        localTransform_[0][row], localTransform_[1][row], 
        localTransform_[2][row], localTransform_[3][row]);
  }

  std::print("{:{}}  Global Transform Matrix:\n", "", indent);
  for (int row = 0; row < 4; ++row) {
    std::print("{:{}}    [{:5.2f}, {:5.2f}, {:5.2f}, {:5.2f}]\n", "", indent,
        globalTransform_[0][row], globalTransform_[1][row], 
        globalTransform_[2][row], globalTransform_[3][row]);
  }

  std::print("{:{}}--------------------\n", "", indent);
  indent+=4;
  if (recurse) {
    for (auto* it : children_) {
      it->printInfo(true, indent);      
    }
  }
}
