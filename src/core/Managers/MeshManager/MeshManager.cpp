
#define TINYOBJLOADER_IMPLEMENTATION
#include "MeshManager.hpp"

#include "glad/glad.h"
#include "Logger/ILogger.hpp"
#include "RenderUtils/RenderTypes.hpp" // for InstanceData
void MeshManager::initializeVAOTemplates(){
  // 3d basic 
  glCreateVertexArrays(1, &vaoTemplate3D_);
  setupVertexAttributes3D(vaoTemplate3D_);
  setupInstanceAttributes(vaoTemplate3D_);

  // billboards (which are. 3d)
  glCreateVertexArrays(1, &vaoTemplate3DBillboard_);
  setupVertexAttributes3D(vaoTemplate3DBillboard_);
  setupInstanceAttributes(vaoTemplate3DBillboard_);
  glVertexArrayElementBuffer(vaoTemplate3DBillboard_, sharedQuadEBO_);

  //ui, 2d
  glCreateVertexArrays(1, &vaoTemplate2D_);
  setupVertexAttributes3D(vaoTemplate2D_);
  setupInstanceAttributes(vaoTemplate2D_);
  glVertexArrayElementBuffer(vaoTemplate2D_, sharedQuadEBO_);

  // ui, 3d!! 
  glCreateVertexArrays(1, &vaoTemplate3DUI_);
  setupVertexAttributes3D(vaoTemplate3DUI_);
  setupInstanceAttributes(vaoTemplate3DUI_);

  //quad
  createSharedQuadBuffers();

}


MeshHandle MeshManager::loadMesh(const std::string& objPath) {
  logger_->log(LogLevel::Info, "atttempting to load mesh: "+objPath);
  auto it = loadedMeshes_.find(objPath);
  if (it != loadedMeshes_.end()) {
    MeshHandle existingHandle = it->second;
    uint32_t index = getIndex(existingHandle.id);

    slots_[index].refCount++;
    logger_->log(LogLevel::Info, "Mesh cached. Incremented refCount! no reloading.");
    return existingHandle;
  }
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, objPath.c_str(), nullptr)) {
    logger_->log(LogLevel::Error, "Failed to load OBJ: " + warn );
    return MeshHandle{0};
  }



  std::vector<Vertex3D> vertices;
  std::vector<uint32_t> indices;

  std::map<Vertex3D, uint32_t> uniqueVertices; 
  glm::vec3 minBounds(std::numeric_limits<float>::max());
  glm::vec3 maxBounds(std::numeric_limits<float>::lowest());



  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      Vertex3D vertex{};

      vertex.position = {
        attrib.vertices[3 * index.vertex_index + 0],
        attrib.vertices[3 * index.vertex_index + 1],
        attrib.vertices[3 * index.vertex_index + 2]
      };

      // Track bounding box boundaries
      minBounds = glm::min(minBounds, vertex.position);
      maxBounds = glm::max(maxBounds, vertex.position);

      if (index.normal_index >= 0) {
        vertex.normal = {
          attrib.normals[3 * index.normal_index + 0],
          attrib.normals[3 * index.normal_index + 1],
          attrib.normals[3 * index.normal_index + 2]
        };
      } else {
        vertex.normal = {0.0f, 1.0f, 0.0f};
      }

      if (index.texcoord_index >= 0) {
        vertex.uv = {
          attrib.texcoords[2 * index.texcoord_index + 0],
          1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
        };
      } else {
        vertex.uv = {0.0f, 0.0f};
      }

      vertex.tangent = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); 

      if (uniqueVertices.count(vertex) == 0) {
        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }

      indices.push_back(uniqueVertices[vertex]);
    }
  }

  // Populate your specific MeshData fields without creating a VAO
  MeshData loadedMeshData;

  glCreateBuffers(1, &loadedMeshData.vbo);
  glNamedBufferStorage(loadedMeshData.vbo, vertices.size() * sizeof(Vertex3D), vertices.data(), 0);

  glCreateBuffers(1, &loadedMeshData.ebo);
  glNamedBufferStorage(loadedMeshData.ebo, indices.size() * sizeof(uint32_t), indices.data(), 0);

  loadedMeshData.indexCount = static_cast<uint32_t>(indices.size());
  loadedMeshData.vertexCount = static_cast<uint32_t>(vertices.size());
  loadedMeshData.bounds = BoundingBox{ glm::vec4(minBounds,0), glm::vec4(maxBounds,0) };
  loadedMeshData.vaoType = VAOType::Standard3D; // Mark this asset type

  return allocateHandle(loadedMeshData);
  return allocateHandle(loadedMeshData);
}


MeshHandle MeshManager::allocateHandle(const MeshData &toAlloc, const std::string& path){ 
  uint32_t targetIndex = 0;
  uint16_t nextGeneration = 1;

  // if not, recycle or append
  if (!freeSlots_.empty()) {
    targetIndex = freeSlots_.back();
    freeSlots_.pop_back();
    nextGeneration = slots_[targetIndex].generation + 1;
    if (nextGeneration == 0) nextGeneration = 1; // skip 0 for. in case
  } else {
    // none? grow
    targetIndex = static_cast<uint32_t>(slots_.size());
    slots_.emplace_back(); 
  }

  MeshResource& slot = slots_[targetIndex];
  slot.data = toAlloc;
  slot.generation = nextGeneration;
  slot.refCount = 1;
  slot.isFree = false;

  MeshHandle handle;
  handle.id = (static_cast<uint32_t>(nextGeneration) << 16) | (targetIndex & 0xFFFF);

  if (!path.empty()) {
    loadedMeshes_[path] = handle;
  }

  return handle;
}


MeshData* MeshManager::getMesh(MeshHandle handle) {
  uint32_t index = getIndex(handle.id);
  uint32_t handleGen = getGeneration(handle.id); 

  if (index >= slots_.size()) {

    logger_->log(LogLevel::Critical, "invlaid mesh handle passed out?");
    return nullptr;
  }

  const MeshResource& slot = slots_[index];
  if (slot.isFree || slot.generation != handleGen) {
    logger_->log(LogLevel::Critical, "something terribly awry in getting mesh!");
    return nullptr;
  }

  return const_cast<MeshData*>(&slot.data);
}
void MeshManager::unloadMesh(MeshHandle handle) {
  uint32_t index = getIndex(handle.id);
  uint32_t handleGen = getGeneration(handle.id); 

  if (index >= slots_.size()) return;

  MeshResource& slot = slots_[index];

  if (!slot.isFree && slot.generation == handleGen) {
    slot.refCount--;

    if (slot.refCount == 0) {
      slot.isFree = true;


      logger_->log(LogLevel::Critical, "i never actually implemented unloaded LOL");
      freeSlots_.push_back(index);
    }
  }
}














void MeshManager::initalize(void * logger){
  if (logger){ 
    logger_ = static_cast<ILogger*>(logger);
    logger_->log(LogLevel::Info, "Mesh Managager Initialized!");
  }
  initializeVAOTemplates();
}


void MeshManager::createSharedQuadBuffers() {
  Vertex3D quadVerts[4] = {
    // thats pos,              normal,               uv,        tangent
    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
  };
  uint32_t quadIndices[6] = {0,1,2, 0,2,3};

  glCreateBuffers(1, &sharedQuadVBO_);
  glNamedBufferStorage(sharedQuadVBO_, sizeof(quadVerts), quadVerts, 0);

  glCreateBuffers(1, &sharedQuadEBO_);
  glNamedBufferStorage(sharedQuadEBO_, sizeof(quadIndices), quadIndices, 0);
}

void MeshManager::setupVertexAttributes3D(GLuint vao) {
  // position 
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, position));
  glVertexArrayAttribBinding(vao, 0, 0);

  // normal 
  glEnableVertexArrayAttrib(vao, 1);
  glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, normal));
  glVertexArrayAttribBinding(vao, 1, 0);

  // uv
  glEnableVertexArrayAttrib(vao, 2);
  glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, uv));
  glVertexArrayAttribBinding(vao, 2, 0);

  // tangent 
  glEnableVertexArrayAttrib(vao, 3);
  glVertexArrayAttribFormat(vao, 3, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, tangent));
  glVertexArrayAttribBinding(vao, 3, 0);
}

void MeshManager::setupInstanceAttributes(GLuint vao) {
  // modelMatrix - 4 vec4s  = 1 4x4 matrix
  for (int i = 0; i < 4; i++) {
    glEnableVertexArrayAttrib(vao, 4 + i);
    glVertexArrayAttribFormat(vao, 4 + i, 4, GL_FLOAT, GL_FALSE,
        offsetof(InstanceData, modelMatrix) + i * sizeof(glm::vec4));
    glVertexArrayAttribBinding(vao, 4 + i, 1);
    glVertexArrayBindingDivisor(vao, 1, 1);
  }

  // materialIndex  (INT!!) 
  glEnableVertexArrayAttrib(vao, 8);
  glVertexArrayAttribIFormat(vao, 8, 1, GL_UNSIGNED_INT, offsetof(InstanceData, materialIndex));
  glVertexArrayAttribBinding(vao, 8, 1);
  glVertexArrayBindingDivisor(vao, 1, 1);

  // entityID INT!! 
  glEnableVertexArrayAttrib(vao, 9);
  glVertexArrayAttribIFormat(vao, 9, 1, GL_UNSIGNED_INT, offsetof(InstanceData, entityID));
  glVertexArrayAttribBinding(vao, 9, 1);
  glVertexArrayBindingDivisor(vao, 1, 1);
}
