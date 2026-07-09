#pragma once
#include <memory>
#include <stdexcept>
#include "Engine/Engine.hpp"
#include "Managers/InputManager/CoreInputManager.hpp"
#include "Managers/InputManager/InputAid/Input.hpp"
#include "RenderServer/RenderServer.hpp"
#include "Logger/LoggerBasic.hpp"
#include "WindowServer/OpenGLWindowServer.hpp"
#include "commandSource/TermInputService.hpp"
#include "SceneManager/SceneManager.hpp"
#include "InterpolateServer/InterpolateServer.hpp"
struct  wEngApplication {
  // destroyed bottom to top
  wEngApplication() {
    
    //allocate, then share
    logger_ = std::make_unique<LoggerBasic>();
    // commandSource_ = std::make_unique<TerminalInputService>();
    inputManager_ = std::make_unique<CoreInputManager>(logger_.get());
    
    inputStaticAccess_ = std::make_unique<Input>();
    inputStaticAccess_->sInstance_=inputManager_.get();

    sceneManager_ = std::make_unique<SceneManager>();
    windowServer_ = std::make_unique<OpenGLWindowServer>(inputManager_.get(), logger_.get());
    interpolateserver_ = std::make_unique<InterpolateServer>();
    renderServer_ = std::make_unique<RenderServer>(windowServer_.get(), interpolateserver_.get(), sceneManager_.get(), logger_.get());
    
    if (!renderServer_->init("OpenGL Revision", 1280, 720)) {
      logger_->log(LogLevel::Critical, "Failed To Initialize Server!!");
      std::cerr<<"fallback failurue"<<std::endl;
    }
    engine_ = std::make_unique<Engine>(renderServer_.get(), inputManager_.get(), sceneManager_.get(), logger_.get());
    // engine_ = std::make_unique<Engine>(renderServer_.get(), inputManager_.get(), commandSource_.get(), logger_.get());
  }

  std::unique_ptr<ILogger> logger_;
  // std::unique_ptr<ICommandSource> commandSource_;
  std::unique_ptr<IInputManager>  inputManager_;
  std::unique_ptr<Input> inputStaticAccess_;

  std::unique_ptr<ISceneManager> sceneManager_;
  
  std::unique_ptr<OpenGLWindowServer>  windowServer_;
  std::unique_ptr<IInterpolateServer> interpolateserver_;
  std::unique_ptr<IRenderServer> renderServer_;
  std::unique_ptr<IEngine> engine_;

};
