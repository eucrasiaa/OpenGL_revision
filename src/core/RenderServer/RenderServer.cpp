#include "RenderServer.hpp"
#include "RenderServer/WindowServer/IWindowServer.hpp"
#include "InterpolateServer/IInterpolateServer.hpp"
void RenderServer::shutdown(){
  return;
}


bool RenderServer::init(const char* title, int width, int height){
  // windowServer_->InitializeWindow(title, width, height, WindowMode::Windowed);
  windowServer_->InitializeWindow( {
      .title     = "OpenGL windowServer title",
      });
  return true;
}

void RenderServer::render(double alpha, double elapsedTime){
  return;
}

void RenderServer::advanceInterpolate() {
  interpolateServer_->advanceTick();
  return;
}


bool RenderServer::pollServer() {
  return windowServer_->PollEvents();
}

void RenderServer::resolveAllTransforms(double alpha){
  for (auto& e : renderEntities_) {
    e.resolved = false;
  }
  //cache to prevent duplicate 
  for (auto& e : renderEntities_) {
    resolveWorldTransform(e.id, alpha);
  }
}


// non template
void RenderServer::resolveWorldTransform(EntityID id, double alpha) {
  RenderEntity& entity = renderEntities_[id];
  if (entity.resolved) return; // already computed this frame

  //local interpolated matrix
  glm::mat4 localMatrix = interpolateServer_->getInterpolatedLocalMatrix(id, alpha);

  // parent's world matrix (recurse up)
  glm::mat4 parentWorld = glm::mat4(1.0f); // identity for root
  if (entity.parentID != 0) {
    resolveWorldTransform(entity.parentID, alpha); // recursive resolution
    parentWorld = renderEntities_[entity.parentID].resolvedWorldMatrix;
  }

  // merge
  entity.resolvedWorldMatrix = parentWorld * localMatrix;
  entity.resolved = true;
}


