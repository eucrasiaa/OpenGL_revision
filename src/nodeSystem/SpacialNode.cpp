#include "SpacialNode.hpp"

void SpacialNode::checkCalculate(const glm::mat4& parentTransform, bool parentIsDirty) {

  if (parentIsDirty || isDirty_) {
    computeTransforms(parentTransform);
  }
  // RenderInstance instance;
  // instance.globalTransform = m_globalTransform;
  // instance.textureHandle = m_textureHandle;
  // instance.layer = static_cast<int>(m_renderLayer);
  // m_renderServer->SubmitInstance(instance);

  // Render children

  // for (auto* child : children_) {
  //   child->checkCalculate(globalTransform_, isDirty_);
  // }

  isDirty_ = false;
}


void SpacialNode::computeTransforms(const glm::mat4& parentTransform) {
  // first convert quaentern ? to 4x4 rot 
  glm::mat4 rotMatrix = glm::mat4_cast(quaternion_);

  // local transform = Translate * Rotate * Scale 
  localTransform_ = glm::translate(glm::mat4(1.0f), position_) 
    * rotMatrix 
    * glm::scale(glm::mat4(1.0f), scale_);


  //global = parent * local 
  globalTransform_ = parentTransform * localTransform_;
  //update basis 
  right_ = glm::normalize(glm::vec3(globalTransform_[0]));
  up_ = glm::normalize(glm::vec3(globalTransform_[1]));

  // consider it might be backwasrds ?
  forward_ = glm::normalize(glm::vec3(globalTransform_[2]));
  
  isDirty_ = false;

}
void SpacialNode::setLocalTransform(const glm::mat4& transform) {
  localTransform_ = transform;
  isDirty_ = true;
}


void SpacialNode::setPosition(const glm::vec3& position) {
  localTransform_[3] = glm::vec4(position, 1.0f);
  isDirty_ = true;
}
void SpacialNode::setRotation(const glm::quat& rotation) {
  //TODO

  isDirty_ = true;
}
void SpacialNode::setScale(const glm::vec3& scale) {
  //TODO

  isDirty_ = true;
}
void SpacialNode::markDirty() {
  isDirty_ = true;
}
void SpacialNode::printInfo(bool recurse, int indent) {
  std::print("{:{}}Spacial Node:\n", "", indent);
  std::print("{:{}}  Children: {}\n", "", indent, children_.size());
  std::print("{:{}}  Parent: {:#x}\n", "", indent, 
      (parent_ != nullptr) ? reinterpret_cast<std::uintptr_t>(parent_) : 0);
  std::print("{:{}}  Dirty State: {}\n", "", indent, isDirty_ ? "true" : "false");

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
