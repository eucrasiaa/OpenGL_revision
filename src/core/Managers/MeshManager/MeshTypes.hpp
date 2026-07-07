#pragma once

#include <glm/ext/vector_float4.hpp>
#include <glad/glad.h>
struct MeshHandle { uint32_t id; };

struct MaterialHandle { 
  uint32_t id; // to MeshManager 
               // finds VAO, VBO, indexCount, vertex format
};

//TEMP!!! 
// using vec4s even though they should only need vec3 for padding stuff 
// AABB stuff
struct BoundingBox {
    glm::vec4 center;  // midpoint 
    glm::vec4 extents; // half-widths (dist center -> edges)
};

// bb box center to world space 
// cull with AABB against camera?


// i assume this looks like the thing that builds uniforms?
struct VertexAttribute {
    GLuint location; // glsl location
    GLint size;      // comp count  
    GLenum type;     // GL_FLOAT, GL_HALF_FLOAT
    GLboolean normalized;
    GLuint relativeOffset; // Offset from the start of the vertex data
};
struct VertexFormat{
    uint32_t stride;     // bytes per vertex 
    std::vector<VertexAttribute> attributes;
};

struct MeshData {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  uint32_t indexCount;
  GLenum indexType;
  VertexFormat format;
  BoundingBox bounds; //culling.
};

// std430
#pragma pack(push, 1) 
struct Material {
    uint64_t albedoTexture             = 0; // glGetTextureHandleARB()
    uint64_t normalTexture             = 0;
    uint64_t metallicRoughnessTexture  = 0;
    uint32_t pipelineID                = 0;
    float    metallic                  = 0.0f;
    float    roughness                 = 1.0f;
    glm::vec4 albedoColor              = glm::vec4(1.0f);
};
#pragma pack(pop)

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
