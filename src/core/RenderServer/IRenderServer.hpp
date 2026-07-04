#pragma once
#include "RenderServer/RenderTypes.hpp"
#include <glm/glm.hpp>

class IRenderServer {
public:
    virtual ~IRenderServer() = default;
    
    //lifecycle
    virtual bool init(const char* title, int width, int height) = 0;
    virtual void shutdown() = 0;
    virtual void render(double dt) = 0;
    
    //rendering
    virtual void SubmitInstance(const RenderInstance& instance) = 0;
    virtual void ClearQueues() = 0;
    
    // Camera controls
    virtual void setCamera(const glm::vec3& pos, const glm::vec3& look, const glm::vec3& up) = 0;
    
    // virtual int changeResolution(int width, int height) = 0;
    
    // // Configuration
    // virtual void setPostProcessMode(const std::string& mode) = 0;
    // virtual bool getOnly2DMode() const = 0;
};

// IEngine.hpp (optional, if you want to abstract Engine too)

