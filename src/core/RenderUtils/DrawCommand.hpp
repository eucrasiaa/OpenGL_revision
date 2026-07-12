#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <Managers/MeshTypes.hpp>

#include <Managers/MaterialTypes.hpp>
#include <NodeTypes.hpp>


enum class RenderLayer : uint8_t {
  RIL_OPAQUE             = 3,
  RIL_TRANSPARENT        = 4,
  RIL_UI_3D              = 5,
  RIL_UI_2D              = 6,
  RIL_SKYBOX             = 7,
  RIL_POSTPROCESS_WORLD  = 8,
  RIL_POSTPROCESS_SCREEN = 9,
  RIL_BILLBOARD_2D       = 10,
  RIL_CAMERA             = 11
};

// struct EntityID{
//   uint64_t id;
// };


enum DrawCmdFlags : uint8_t {
    None           = 0,
    Visible        = 1 << 0,  // 1
    CastShadows    = 1 << 1,  // 2
    ReceiveShadows = 1 << 2,  // 4
    IsBillboard    = 1 << 3   // 8
};

struct DrawCommand {
    EntityID entityID; // for interpolation lookup
    MeshHandle mesh;
    MaterialHandle material;
    RenderLayer layer;

    uint32_t pipelineProgram = 0; // shader pipeline id
    glm::vec3 boundsOffset = glm::vec3(0.0f);
    float boundsScale = 1.0f;
    uint8_t drawCommandFlags = Visible | CastShadows | ReceiveShadows;
    // transform from InterpolateService
    // VAO / index from MeshHandle
    // textureID from Material
};

struct DrawCommandWithTransform {
    DrawCommand base;
    glm::mat4 overrideTransform; // for no interpolation (UI, debug)
    bool useExplicitTransform;
};
