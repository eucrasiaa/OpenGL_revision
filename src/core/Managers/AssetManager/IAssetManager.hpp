#pragma once
#include <string>
#include <memory>
#include "MeshManager/IMeshManager.hpp"
#include "TextureManager/ITextureManager.hpp"
#include "MaterialManager/IMaterialManager.hpp"
class ILogger;
class IAssetManager {
  public:
    virtual ~IAssetManager() = default;
    // IMeshManager* meshManager = nullptr;
    // IMaterialManager* materialManager = nullptr;
    // ITextureManager* textureManager = nullptr;

    std::unique_ptr<IMeshManager> meshManager_ = nullptr;
    std::unique_ptr<IMaterialManager> materialManager_ = nullptr;
    std::unique_ptr<ITextureManager> textureManager_ = nullptr;
    ILogger *logger_ = nullptr;

    virtual MeshHandle GetOrLoadMesh(const std::string& path) = 0;
    virtual MaterialHandle GetOrCreateMaterial(const std::string& name,
        const std::string& albedoPath,
        const std::string& normalPath = "",
        const std::string& metallicRoughnessPath = "") = 0;

    virtual MaterialHandle GetDefaultMaterial() = 0; // White, no textures
    virtual MaterialHandle GetErrorMaterial() = 0;   // Magenta, highly visible

    IAssetManager(IMeshManager* meshm, IMaterialManager* matm, ITextureManager* texm, ILogger* logger) : meshManager_(meshm), materialManager_(matm), textureManager_(texm), logger_(logger) {
      if(meshManager_ == nullptr){ throw std::runtime_error("AssetManager: failed to pass a mesh manager");}
      if(materialManager_ == nullptr){ throw std::runtime_error("AssetManager: failed to pass a materialManager ");}
      if(textureManager_ == nullptr){ throw std::runtime_error("AssetManager: failed to pass a texture manager");} 
    }
    virtual void init(){ 
      meshManager_->initalize(logger_);
      textureManager_->init(logger_);
      materialManager_->initialize(logger_);
    }

};
