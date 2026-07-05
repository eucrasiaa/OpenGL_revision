#pragma once
#include <memory>
#include <stdexcept>
#include "Engine/Engine.hpp"
#include "RenderServer/RenderServer.hpp"
#include "Logger/LoggerBasic.hpp"

struct  wEngApplication {
  // destroyed bottom to top
  wEngApplication() {
    
    //allocate, then share
    logger_ = std::make_unique<LoggerBasic>();

    renderServer_ = std::make_unique<RenderServer>();
    
    if (!renderServer_->init("OpenGL Revision", 1280, 720)) {
      logger_->log(LogLevel::Critical, "Failed To Initialize Server!!");
      std::cerr<<"fallback failurue"<<std::endl;
    }
    engine_ = std::make_unique<Engine>(renderServer_.get(),logger_.get());
  }

  std::unique_ptr<ILogger> logger_;
  std::unique_ptr<IRenderServer> renderServer_;
  std::unique_ptr<IEngine> engine_;

};
