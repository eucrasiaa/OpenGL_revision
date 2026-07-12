#pragma once
#include "IAssetManager.hpp"


class AssetManager : public IAssetManager{
  private: 
    std::unordered_map<std::string, MeshHandle> meshCache_;
    std::unordered_map<std::string, MaterialHandle> materialCache_;

    MaterialHandle defaultMaterial_;
    MaterialHandle errorMaterial_;
    bool defaultsCreated_ = false;

  public:

    //todo CAche stuff? idk seems excessive
    virtual MeshHandle GetOrLoadMesh(const std::string& path) override;
    virtual MaterialHandle GetOrCreateMaterial(const std::string& name,
        const std::string& albedoPath,
        const std::string& normalPath = "",
        const std::string& metallicRoughnessPath = "") override;

    virtual MaterialHandle GetDefaultMaterial() override; // White, no textures
    virtual MaterialHandle GetErrorMaterial() override;   // Magenta, highly visible

    virtual void init() override; 
};
