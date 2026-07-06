#include "RenderServer.hpp"
#include "RenderServer/WindowServer/IWindowServer.hpp"

void RenderServer::shutdown(){
  return;
}


bool RenderServer::init(const char* title, int width, int height){
  windowServer_->InitializeWindow(title, width, height, WindowMode::Windowed);
  return true;
}

void RenderServer::render(double dt){
  return;
}



bool RenderServer::pollServer() {
  return windowServer_->PollEvents();
}
