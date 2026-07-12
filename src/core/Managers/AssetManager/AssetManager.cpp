#include "AssetManager.hpp"
#include "AssetManager/IAssetManager.hpp"
#include "Logger/ILogger.hpp"

MeshHandle AssetManager::GetOrLoadMesh(const std::string& path){

  return meshManager_->loadMesh(path);

}
MaterialHandle AssetManager::GetOrCreateMaterial(const std::string& name,
    const std::string& albedoPath,
    const std::string& normalPath,
    const std::string& metallicRoughnessPath){

  return materialManager_->createMaterial(*textureManager_, albedoPath, normalPath, metallicRoughnessPath);
}

MaterialHandle AssetManager::GetDefaultMaterial(){
  if (defaultsCreated_){
    return defaultMaterial_;
  }
  else{ 
    logger_->log(LogLevel::Warning,"Attempted get default material without initializing?"); 
    init();
    return defaultMaterial_;
  }
}
MaterialHandle AssetManager::GetErrorMaterial(){
  if (defaultsCreated_){
    return errorMaterial_;
  }
  else{ 
    logger_->log(LogLevel::Warning,"Attempted get error material without initializing?"); 
    init();
    return errorMaterial_;
  }
}

void AssetManager::init(){
  IAssetManager::init();
  defaultMaterial_= materialManager_->createColoredMaterial(glm::vec4(1,1,1,1)); 
  errorMaterial_= materialManager_->createColoredMaterial(glm::vec4(1,0,1,1)); 
  defaultsCreated_=true;

} 
