#pragma once
#include "RenderUtils/RenderTypes.hpp"
#include <glm/glm.hpp>

class IRenderServer {
public:
    virtual ~IRenderServer() = default;
    //lifecycle
    virtual bool init(const char* title, int width, int height) = 0;
    virtual void shutdown() = 0;
    virtual void render(double dt) = 0;
};

