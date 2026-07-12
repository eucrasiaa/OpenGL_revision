#pragma once
#include "Managers/MaterialTypes.hpp"
#include <string>
#include <string_view>
// job: simply just hands out the bindless Texture IDS ? 

class ITextureManager {
public:
  virtual ~ITextureManager() = default;

  virtual uint64_t loadTexture(std::string_view filePath, bool useLinear = false) = 0;
  virtual void releaseTexture(uint64_t handle) = 0;

  virtual uint64_t loadTexture(const std::string& filePath, bool useLinear = false) {
    return loadTexture(std::string_view(filePath), useLinear); 
  }

  // virtual void releaseTexture(const std::string& filePath) {
  //   releaseTexture(std::string_view(filePath));
  // }
  virtual void init(void * bonus = nullptr) = 0;



};
