#pragma once 

#include "MaterialTypes.hpp"
#include <string>

#include <glad/glad.h>

class ITextureManager;
class IMaterialManager{ 
  public:
    virtual ~IMaterialManager() = default;
    virtual MaterialHandle createMaterial(ITextureManager& texManager,
        const std::string& albedoPath,
        const std::string& normalPath = "",
        const std::string& metallicRoughnessPath = "",
        const std::string& emissivePath = "",
        const std::string& aoPath = "")=0;

    virtual MaterialHandle createColoredMaterial(const glm::vec4& color,
        float metallic = 0.0f,
        float roughness = 1.0f)=0;

    virtual void setMaterialProperties(MaterialHandle handle,
        const glm::vec4& albedoColor,
        float metallic,
        float roughness,
        float ao = 1.0f)=0;

    virtual void setPipelineID(MaterialHandle handle, uint32_t pipelineID)=0;

    virtual uint32_t getSSBOIndex(MaterialHandle handle) const=0;


    virtual const Material* getMaterial(MaterialHandle handle) const = 0;

    virtual void uploadToSSBO(GLuint ssbo) = 0;

    virtual bool isTransparent(MaterialHandle handle) const =0;

    virtual void initialize(void * bonus) = 0; 
};
