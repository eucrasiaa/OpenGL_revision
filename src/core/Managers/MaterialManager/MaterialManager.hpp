#pragma once
#include "IMaterialManager.hpp"
#include "MaterialTypes.hpp"
#include "TextureManager/ITextureManager.hpp"
class ILogger;
class MaterialManager : public IMaterialManager{
  
  private: 

    struct InternalMaterial {
        Material data;
        uint32_t generation = 1;
        uint32_t ssboIndex = 0;
        bool transparent = false;
    };

    static constexpr const uint64_t INDEX_MASK = 0x00000000FFFFFFFF;
    static constexpr const uint64_t GEN_MASK = 0xFFFFFFFF00000000;
    [[nodiscard]] static constexpr inline uint32_t getIndex(uint64_t val) noexcept {
      return static_cast<uint32_t>(val & INDEX_MASK);
    }

    [[nodiscard]] static constexpr inline uint32_t getGeneration(uint64_t val) noexcept {
      return static_cast<uint32_t>((val & GEN_MASK) >> 32);
    }


    ILogger* logger_ = nullptr;
    
    std::vector<InternalMaterial> materials_;
    std::vector<uint32_t> freeList_;
    MaterialHandle allocateSlot();
    void updateTransparencyFlag(InternalMaterial& mat);

  public:

    virtual MaterialHandle createMaterial(ITextureManager& texManager,
        const std::string& albedoPath,
        const std::string& normalPath = "",
        const std::string& metallicRoughnessPath = "",
        const std::string& emissivePath = "",
        const std::string& aoPath = "") override;

    virtual MaterialHandle createColoredMaterial(const glm::vec4& color,
        float metallic = 0.0f,
        float roughness = 1.0f) override;

    virtual void setMaterialProperties(MaterialHandle handle,
        const glm::vec4& albedoColor,
        float metallic,
        float roughness,
        float ao = 1.0f) override;

    virtual void setPipelineID(MaterialHandle handle, uint32_t pipelineID) override;

    virtual uint32_t getSSBOIndex(MaterialHandle handle) const override;


    virtual const Material* getMaterial(MaterialHandle handle) const override;

    virtual void uploadToSSBO(GLuint ssbo) override;

    virtual bool isTransparent(MaterialHandle handle) const override;
    virtual void initialize(void * logger) override;

};
