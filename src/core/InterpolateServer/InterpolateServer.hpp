#pragma once

#include "InterpolateServer/IInterpolateServer.hpp"
#include "InterpolateTypes.hpp"
class InterpolateServer : public IInterpolateServer {
  private:
    std::vector<Buffer> buffers_;
    glm::mat4 makeLocalMatrix(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl) const;

    glm::mat4 makeLocalMatrix(const LocalSnapshot& s) const;

  public:
    virtual void capture(EntityID id, const glm::vec3& localPos, const glm::quat& localRot, const glm::vec3& localScale);
    virtual void advanceTick();
    // to find INTERPOLATED LOCAL MATRIX
    virtual glm::mat4 getInterpolatedLocalMatrix(EntityID id, double alpha) const;
};
