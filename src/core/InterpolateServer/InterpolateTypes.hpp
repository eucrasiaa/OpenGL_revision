#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/vector_float3.hpp>
struct LocalSnapshot {
  glm::vec3 position;  // LOCAL position
  glm::quat rotation;  // LOCAL rotation
  glm::vec3 scale;     // LOCAL scale
};

struct Buffer {
  LocalSnapshot prev;
  LocalSnapshot current;
  bool hasPrev = false; // solve first tick interp math
};
