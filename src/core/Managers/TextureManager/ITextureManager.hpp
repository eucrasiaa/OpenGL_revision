#pragma once
#include "Managers/MaterialTypes.hpp"
#include <string>
#include <string_view>
// job: simply just hands out the bindless Texture IDS ? 

class ITextureManager {
public:
  virtual ~ITextureManager() = default;

  virtual TextureHandle loadTexture(std::string_view filePath, bool useLinear = false) = 0;
  virtual void releaseTexture(TextureHandle handle) = 0;

  virtual TextureHandle loadTexture(const std::string& filePath, bool useLinear = false) {
    return loadTexture(std::string_view(filePath), useLinear); 
  }

  // virtual void releaseTexture(const std::string& filePath) {
  //   releaseTexture(std::string_view(filePath));
  // }
  virtual void init(void * bonus = nullptr) = 0;



};
