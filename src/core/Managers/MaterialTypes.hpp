#pragma once
#include <glm/ext/vector_float4.hpp>


// to TextureManager 
// finds Material Struct 
struct MaterialHandle { uint64_t id;};



struct TextureHandle { uint64_t id;};

// // std430
// struct Material {
//     uint64_t albedoTexture             = 0; // glGetTextureHandleARB()
//     uint64_t normalTexture             = 0;
//     uint64_t metallicRoughnessTexture  = 0;
//
//
//     uint32_t pipelineID                = 0;
//
//
//     float    metallic                  = 0.0f;
//     float    roughness                 = 1.0f;
//     float padding[3]; //12 byte gap
//     glm::vec4 albedoColor              = glm::vec4(1.0f);
// };


// std430
struct Material {
    uint64_t albedoTexture             = 0; // glGetTextureHandleARB()
    uint64_t normalTexture             = 0;
    uint64_t metallicRoughnessTexture  = 0; 
    uint64_t emissiveTexture           = 0; 
    uint64_t aoTexture                 = 0; 

    uint32_t pipelineID = 0;


    float metallic          = 0.0f;
    float roughness         = 1.0f;
    float ao                = 1.0f;
    float padding;
    glm::vec4 albedoColor              = glm::vec4(1.0f);
    glm::vec4 emissiveColor            = glm::vec4(0.0f);
    glm::vec4 padding1; // for now? alignment 
    glm::vec4 padding2;  // could be future use too
};
static_assert(sizeof(Material) == 128, "Material must be 128 bytes for cache line alignment");


// #pragma pack(pop)

// layout(std140, binding = 1) uniform MaterialBuffer {
//     Material materials[256];
// };
// struct Material { 
//   // 8 + 8 = 16
//   uint64_t albedoTexture;  // GLUint for regular, 64 for bindless?
//   uint64_t normalTexture;
//
//   // 8 + 4 + 4
//   uint64_t metallicRoughnessTexture;
//   uint32_t pipelineID;
//   uint32_t _padding_1;
//   // 4 + 4 + 8 = 16
//   float metallic;
//   float roughness;
//   uint64_t _padding_2;
//   // 16 = 16
//   glm::vec4 albedoColor;
// };

// struct Material { 
//
//   uint32_t pipelineID;
//   // Textures (bindless handles)
//   uint64_t albedoTexture;  // GLUint for regular, 64 for bindless?
//   uint64_t normalTexture;
//   uint64_t metallicRoughnessTexture;
//   // Uniform data (goes into a material UBO)
//   // Eventually: uniform buffer offset for material properties
//   float metallic;
//   float roughness;
//   float padding;
//   glm::vec4 albedoColor;
// };
