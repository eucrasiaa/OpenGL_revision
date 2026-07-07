#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include "MeshManager/MeshTypes.hpp"
enum class RenderLayer{
  RIL_OPAQUE,
  RIL_TRANSPARENT,
  RIL_UI_3D,
  RIL_UI_2D,
  RIL_SKYBOX,
  RIL_POSTPROCESS_WORLD,
  RIL_POSTPROCESS_SCREEN,
  RIL_CAMERA
};

// struct EntityID{
//   uint64_t id;
// };

typedef uint64_t EntityID;

struct DrawCommand {
    EntityID entityID; // for interpolation lookup
    MeshHandle mesh;
    Material material;
    RenderLayer layer;
    // transform from InterpolateService
    // VAO / index from MeshHandle
    // textureID from Material
};

struct DrawCommandWithTransform {
    DrawCommand base;
    glm::mat4 overrideTransform; // for no interpolation (UI, debug)
    bool useExplicitTransform;
};
