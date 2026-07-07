#pragma once
#include "RenderUtils/RenderTypes.hpp"
#include <glm/glm.hpp>

class IWindowServer;
class IRenderServer {
  protected: 
   IWindowServer * windowServer_ = nullptr; 

public:
    explicit IRenderServer(IWindowServer* windowServer) : windowServer_(windowServer) {
      if (!windowServer_) { throw std::runtime_error("RenderServer's windowServer cannot be null"); }
    }
    virtual ~IRenderServer() = default;
    //lifecycle
    virtual bool init(const char* title, int width, int height) = 0;
    virtual void shutdown() = 0;
    virtual void render(double alpha, double elapsedTime) = 0;
    virtual bool pollServer(){
      return true;
    }

    virtual void resolveAllTransforms(double alpha) {
      return;
    }
};

