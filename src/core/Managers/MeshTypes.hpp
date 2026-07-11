#pragma once
#include <glm/ext/vector_float4.hpp>
#include <glad/glad.h>
 // to MeshManager 
 // finds the MeshData struct
// finds VAO, VBO, indexCount, vertex format
struct MeshHandle { uint32_t id; };



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

