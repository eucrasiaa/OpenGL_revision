#pragma once

#include "InterpolateServer.hpp"

glm::mat4 InterpolateServer::makeLocalMatrix(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl) const{


  return glm::translate(glm::mat4(1.0f), pos) 
    * glm::mat4_cast(rot) 
    * glm::scale(glm::mat4(1.0f), scl);
}


glm::mat4 InterpolateServer::makeLocalMatrix(const LocalSnapshot& s) const{
  return makeLocalMatrix(s.position, s.rotation, s.scale);


}


void InterpolateServer::capture(EntityID id, const glm::vec3& localPos, const glm::quat& localRot, const glm::vec3& localScale) {
  buffers_[id].current = {localPos, localRot, localScale};
}


void InterpolateServer::advanceTick() {
  for (auto& buf : buffers_) {
    buf.prev = buf.current;
    buf.hasPrev = true;
  }

}


// to find INTERPOLATED LOCAL MATRIX
glm::mat4 InterpolateServer::getInterpolatedLocalMatrix(EntityID id, double alpha) const {

  const auto& buf = buffers_[id];
  if (!buf.hasPrev) {
    return makeLocalMatrix(buf.current);
  }
  // mix is = LERP, SLERP is SLERP lol
  glm::vec3 pos = glm::mix(buf.prev.position, buf.current.position, alpha);
  glm::quat rot = glm::slerp(buf.prev.rotation, buf.current.rotation, float(alpha));
  glm::vec3 scl = glm::mix(buf.prev.scale, buf.current.scale, alpha);

  return makeLocalMatrix(pos, rot, scl);

}
