#include "MaterialManager.hpp"

#include "Logger/ILogger.hpp"



void MaterialManager::initialize(void * logger){


  if (logger){ 
    logger_ = static_cast<ILogger*>(logger);
    logger_->log(LogLevel::Info, "Material Managager Initialized!");
  }

}
MaterialHandle MaterialManager::createMaterial(ITextureManager& texManager,
    const std::string& albedoPath,
    const std::string& normalPath,
    const std::string& metallicRoughnessPath,
    const std::string& emissivePath,
    const std::string& aoPath)
{
  Material mat;

  // Load textures and get bindless handles
  if (!albedoPath.empty()) {
    mat.albedoTexture = texManager.loadTexture(albedoPath).id;
  }

  if (!normalPath.empty()) {
    mat.normalTexture = texManager.loadTexture(normalPath).id;
  }

  if (!metallicRoughnessPath.empty()) {
    mat.metallicRoughnessTexture = texManager.loadTexture(metallicRoughnessPath).id;
  }

  if (!emissivePath.empty()) {
    mat.emissiveTexture = texManager.loadTexture(emissivePath).id;
  }

  if (!aoPath.empty()) {
    mat.aoTexture = texManager.loadTexture(aoPath).id;
  }

  mat.pipelineID = 0; // Will be set based on shader requirements
  mat.metallic = 0.0f;
  mat.roughness = 1.0f;
  mat.ao = 1.0f;
  mat.albedoColor = glm::vec4(1.0f);
  mat.emissiveColor = glm::vec4(0.0f);

  uint32_t slot = getIndex(allocateSlot().id);
  materials_[slot].data = mat;
  materials_[slot].generation++;
  materials_[slot].ssboIndex = slot;

  updateTransparencyFlag(materials_[slot]);

  return MaterialHandle{(static_cast<uint64_t>(materials_[slot].generation) << 32) | (slot & INDEX_MASK )};
}


MaterialHandle MaterialManager::createColoredMaterial(const glm::vec4& color,
    float metallic,
    float roughness)
{
  Material mat;
  mat.albedoColor = color;
  mat.metallic = metallic;
  mat.roughness = roughness;
  mat.ao = 1.0f;
  mat.pipelineID = 0;

  uint32_t slot = getIndex(allocateSlot().id);
  materials_[slot].data = mat;
  materials_[slot].generation++;
  materials_[slot].ssboIndex = slot;

  updateTransparencyFlag(materials_[slot]);

  return MaterialHandle{(static_cast<uint64_t>(materials_[slot].generation) << 32) | (slot & INDEX_MASK )};
}



void MaterialManager::updateTransparencyFlag(InternalMaterial& mat) {
  mat.transparent = (mat.data.albedoColor.a < 0.99f);
}




bool MaterialManager::isTransparent(MaterialHandle handle) const {
  uint32_t index = getIndex(handle.id);
  uint32_t generation = getGeneration(handle.id);

  if (index >= materials_.size()) return false;

  const auto& mat = materials_[index];
  if (mat.generation != generation) return false;

  return mat.transparent;
}

uint32_t MaterialManager::getSSBOIndex(MaterialHandle handle) const {
  uint32_t index = getIndex(handle.id);
  uint32_t generation = getGeneration(handle.id);

  if (index >= materials_.size()) return 0;

  const auto& mat = materials_[index];
  if (mat.generation != generation) return 0;

  return mat.ssboIndex;
}

MaterialHandle MaterialManager::allocateSlot() {
  uint32_t index = 0;
  uint32_t nextGeneration = 1;

  if (!freeList_.empty()) {
    index = freeList_.back();
    freeList_.pop_back();


    // copying frorm texturemanager lol
    nextGeneration = materials_[index].generation + 1;
    if (nextGeneration == 0) nextGeneration = 1; // Skip 0
  } else {
    materials_.emplace_back();
    index = static_cast<uint32_t>(materials_.size() - 1);
  }

  materials_[index].generation = nextGeneration;
  materials_[index].ssboIndex = 0; // set during SSBO update
  materials_[index].transparent = false; // default 

  MaterialHandle handle;
  handle.id = (static_cast<uint64_t>(nextGeneration) << 32) | (index & INDEX_MASK);
  return handle;
}

void MaterialManager::uploadToSSBO(GLuint ssbo) {
  std::vector<Material> packedMaterials(materials_.size());
  for (size_t i = 0; i < materials_.size(); ++i) {
    packedMaterials[i] = materials_[i].data;
    materials_[i].ssboIndex = static_cast<uint32_t>(i);
  }

  glNamedBufferSubData(ssbo, 0, 
      packedMaterials.size() * sizeof(Material),
      packedMaterials.data());
}


void MaterialManager::setMaterialProperties(MaterialHandle handle, const glm::vec4& color, float metallic, float roughness, float ao) {
    uint32_t index = getIndex(handle.id);
    uint32_t generation = getGeneration(handle.id);

    if (index >= materials_.size()) return;
    
    auto& mat = materials_[index];
    if (mat.generation != generation) return;

    mat.data.albedoColor = color;
    mat.data.metallic = metallic;
    mat.data.roughness = roughness;
    mat.data.ao = ao;
}

void MaterialManager::setPipelineID(MaterialHandle handle, unsigned int pipelineID) {
    uint32_t index = getIndex(handle.id);
    uint32_t generation = getGeneration(handle.id);

    if (index >= materials_.size()) return;
    
    auto& mat = materials_[index];
    if (mat.generation != generation) return;

    mat.data.pipelineID = pipelineID;
}

const Material* MaterialManager::getMaterial(MaterialHandle handle) const {
    uint32_t index = getIndex(handle.id);
    uint32_t generation = getGeneration(handle.id);

    if (index >= materials_.size()) return nullptr;
    
    const auto& mat = materials_[index];
    if (mat.generation != generation) return nullptr;

    return &mat.data;
}
