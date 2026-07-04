#pragma once
#include <glad/glad.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>
#include <iostream>
#ifdef RENDERINSTANCE_DBG
#include <ansi_control.hpp>
#endif
struct Pipeline {
  GLuint programID = 0;
};

struct PipelineConfig {
  std::string name;
  std::string vertexPath;
  std::string fragmentPath;
};

enum class RenderPassType { World, UI_Flat, UI_3D };
const size_t MAX_SPRITE_COUNT = 10000;
const size_t MAX_VERTEX_COUNT = MAX_SPRITE_COUNT * 4;

struct Vertex {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 uv;
  glm::vec3 normal;
  float texIndex;
};


enum class RenderLayer{
  Opaque,
  Transparent,
  Ui_3d,
  Ui_2d,
  Skybox,
  Postprocess_world,
  Postprocess_screen,
  Camera
};

enum SamplerType {
  LINEAR=true,
  NEAREST=false,
};
using RenderItemID = uint32_t;

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/string_cast.hpp> // Required to print glm::mat4
struct RenderInstance { 
  RenderItemID id; //who
  glm::mat4 globalTransform; // where
  GLuint textureID = 0; // which texture 
  GLuint vaoID=0; // the shape (assumes 0 for universal for now)
  GLuint indexCount = 6;
  std::string pipelineName = "MVP";
  RenderLayer layer = RenderLayer::Opaque;
  bool useCustomViewport = false;
  int vpX=0, vpY=0, vpWidth=0, vpHeight;
  bool isDirty = true;

#ifdef RENDERINSTANCE_DBG
  friend std::ostream& operator<<(std::ostream& os, const RenderInstance& ri) {
    os << "RenderInstance {\n"
      << "  id: " << ri.id << ",\n"
      << "  globalTransform: " << glm::to_string(ri.globalTransform) << ",\n"
      << "  textureID: " << ri.textureID << ",\n"
      << "  vaoID: " << ri.vaoID << ",\n"
      << "  indexCount: " << ri.indexCount << ",\n"
      << "  pipelineName: \"" << ri.pipelineName << "\",\n"
      << "  layer: " << static_cast<int>(ri.layer) << ",\n"
      << "  isDirty: " << (ri.isDirty ? "true" : "false") << "\n"
      << "}";
    // term::Output::move_up(10,true);
    return os;
  }
#endif
};
