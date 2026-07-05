#pragma once 
#include "RenderUtils/DrawCommand.hpp"
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
class InterpolateServer {
private:
    struct TransformSnapshot {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        uint64_t tick;
    };
    
    // [previous, current]
    struct InterpolationBuffer {
        TransformSnapshot snapshots[2];
        int writeIndex = 0;
    };
    
    std::unordered_map<EntityID, InterpolationBuffer> buffers_;
    uint64_t currentTick_ = 0;
    
public:
    // call post update loop
    void advanceTick();
    
    // pre-render , post compute
    void captureTransform(EntityID id, 
                          const glm::vec3& pos, 
                          const glm::quat& rot, 
                          const glm::vec3& scale);
    
    // renderServer -> calls during render
    glm::mat4 getRenderTransform(EntityID id, double alpha) const;
    
    // things without interp (ui stuff)
    void setStaticTransform(EntityID id, const glm::mat4& transform);
};
