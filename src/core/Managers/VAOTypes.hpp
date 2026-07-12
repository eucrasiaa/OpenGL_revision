#pragma once 
#include <glm/ext/matrix_float4x4.hpp>


enum class VAOType{ 
  Standard3D,  // opaque + transparent. anything in 3d space
  Billboard3D, // just diff shader?  anything in 3d space w/ billboard
  Standard2D,  // UI 2d elements! 
  UISpace3D, // 3d elems in ui!
};


// VAO 1, 3d item:
struct Vertex3D {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 tangent;
};

// struct Vertex3D {
//     glm::vec3 position;
//     float padding1;
//     glm::vec3 normal;
//     float padding2;
//     glm::vec2 uv;
//     float padding3[2];
//     glm::vec4 tangent;
// };
// VAO 2: 2d UI 
struct Vertex2D {
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;
};

// Billboard uses 3d but has a single quad vbo 

// all VAOs have this 
struct InstanceData {
    glm::mat4 modelMatrix;
    uint32_t materialIndex;
    uint32_t entityID;
    uint32_t padding[2];
};
