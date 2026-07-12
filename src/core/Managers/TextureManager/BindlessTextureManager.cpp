#include "BindlessTextureManager.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Logger/ILogger.hpp"
void BindlessTextureManager::init(void * Logger){

  if (Logger){ 
    logger_ = static_cast<ILogger*>(Logger);
    logger_->log(LogLevel::Info, "Bindless Texture Managager Initialized!");
  }
  //Linear sampler 
  glCreateSamplers(1, &linearSampler_);
  glSamplerParameteri(linearSampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glSamplerParameteri(linearSampler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(linearSampler_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(linearSampler_, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // nearest smapler
  glCreateSamplers(1, &nearestSampler_);
  glSamplerParameteri(nearestSampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glSamplerParameteri(nearestSampler_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glSamplerParameteri(nearestSampler_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(nearestSampler_, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

TextureHandle BindlessTextureManager::loadTexture(std::string_view filePath, bool useLinear){ 

  std::string lookupKey;
  lookupKey.reserve(filePath.size() + 1);
  lookupKey.append(filePath);
  lookupKey.push_back(useLinear ? 'L' : 'N');
  auto it = textureCache_.find(lookupKey);
  if (it != textureCache_.end()) {
    logger_->log(LogLevel::Info, "Image already found: " + std::string(filePath));
    it->second.refCount++;
    return {it->second.handle};
  }
  logger_->log(LogLevel::Info, "Loading new image: " + std::string(filePath));
  int localWidth = 0;
  int localHeight = 0;
  int localChannels = 0;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* imageData = stbi_load(std::string(filePath).c_str(), &localWidth, &localHeight, &localChannels, 0);
  if (!imageData) {
    logger_->log(LogLevel::Critical, "Failed to laod image!! name was " + std::string(filePath));
    // std::cerr<<"failed to load image, name was: "<<filePath<<"\n";
    return {0}; 
  }

  logger_->log(LogLevel::Info, std::format("\tW: {}  H: {}  C: {}, Linear? {}",localWidth, localHeight,localChannels, useLinear));

  // dsa creation?
  GLuint textureID = 0;
  glCreateTextures(GL_TEXTURE_2D, 1, &textureID); 

  GLenum internalFormat = GL_RGB8;
  GLenum dataFormat = GL_RGB;
  if (localChannels == 1) { internalFormat = GL_R8; dataFormat = GL_RED; }
  else if (localChannels == 3) { internalFormat = GL_RGB8; dataFormat = GL_RGB; }
  else if (localChannels == 4) { internalFormat = GL_RGBA8; dataFormat = GL_RGBA; }


  // uploading 
  //  prep
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  //  alloc
  glTextureStorage2D(textureID, 1, internalFormat, localWidth, localHeight); 
  //  push!
  glTextureSubImage2D(textureID, 0, 0, 0, localWidth, localHeight, dataFormat, GL_UNSIGNED_BYTE, imageData); 

  stbi_image_free(imageData);
  // glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  // glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT); 
  // glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
  // glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
  GLuint samplerToUse = useLinear ? linearSampler_ : nearestSampler_;

  // gen handle for Sampler + texture combo! 
  GLuint64 handle = glGetTextureSamplerHandleARB(textureID, samplerToUse);
  // then make it resident:
  glMakeTextureHandleResidentARB(handle);


  // add to cache 
  TextureResource newResource{ textureID, handle, 1 };
  textureCache_[std::move(lookupKey)] = newResource;

  handleToKeyMap_[handle] = lookupKey;

  return {handle};
}
void BindlessTextureManager::releaseTexture(TextureHandle handle) {
  auto handleIt = handleToKeyMap_.find(handle.id); 
  if (handleIt == handleToKeyMap_.end()) {
    logger_->log(LogLevel::Warning, "attempted to release handle, but not found in list!");
    return; 
  }

  const std::string& cacheKey = handleIt->second;

  auto cacheIt = textureCache_.find(cacheKey);
  if (cacheIt == textureCache_.end()) {
    logger_->log(LogLevel::Error, "handle release failure, not located in main cache!");
    return; 
  }

  cacheIt->second.refCount--;

  if (cacheIt->second.refCount == 0) {
    // remove it FIRST before deleting
    glMakeTextureHandleNonResidentARB(cacheIt->second.handle);
    glDeleteTextures(1, &cacheIt->second.id);
    textureCache_.erase(cacheIt);
    handleToKeyMap_.erase(handleIt);
  }
  return;
}
