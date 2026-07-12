#pragma once 
#include "ITextureManager.hpp"
#include <glad/glad.h>

class ILogger;
class BindlessTextureManager : public ITextureManager {
  private:

    struct TextureResource {
      GLuint id;
      GLuint64 handle;
      unsigned int refCount;
    };


  GLuint linearSampler_ = 0;
  GLuint nearestSampler_ = 0;
  
  std::unordered_map<std::string, TextureResource> textureCache_;
  std::unordered_map<uint64_t, std::string> handleToKeyMap_;
  ILogger *logger_ = nullptr;

  public: 
    virtual TextureHandle loadTexture(std::string_view filePath, bool useLinear = false) override;
    virtual void releaseTexture(TextureHandle handle) override;

    virtual void init(void * Logger=nullptr) override;
};
