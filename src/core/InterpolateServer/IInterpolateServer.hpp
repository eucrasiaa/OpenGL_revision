#pragma once

#include "RenderUtils/DrawCommand.hpp"
#include <glm/ext/quaternion_float.hpp>
class IInterpolateServer{
  public:
    virtual ~IInterpolateServer() = default;
    virtual void capture(EntityID id, const glm::vec3& localPos, const glm::quat& localRot, const glm::vec3& localScale) = 0;
    virtual void advanceTick() = 0;

    // to find INTERPOLATED LOCAL MATRIX
    virtual glm::mat4 getInterpolatedLocalMatrix(EntityID id, double alpha) const = 0;
};
