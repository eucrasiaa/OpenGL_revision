#pragma once
#include "Managers/MaterialTypes.hpp"
#include <string>

// job: simply just hands out the bindless Texture IDS ? 

class ITextureManager {
public:
  virtual ~ITextureManager() = default;
  virtual uint64_t loadTexture(const std::string& filePath, bool useLinear = false) = 0;
  virtual void releaseTexture(const std::string& filePath) = 0;




};
