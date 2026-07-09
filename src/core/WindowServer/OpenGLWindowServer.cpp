#include "OpenGLWindowServer.hpp"
#include "InputManager/IInputManager.hpp"
#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include "Logger/ILogger.hpp"
#include "WindowServer/IWindowServer.hpp"

// Templated 
// bool OpenGLWindowServer::InitializeWindow(const std::string& title, int width, int height, WindowMode mode) {
//   windowWidth_ = width;
//   windowHeight_ = height;
//   windowTitle_=title;
//   // set pointer, uniquetr needs reset
//   SDLWindow_.reset(SDL_CreateWindow(windowTitle_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth_, windowHeight_, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
//
//   openGLSDLContext_.reset(SDL_GL_CreateContext(SDLWindow_.get()));
//   return true;
// }

bool OpenGLWindowServer::InitializeWindow(const WindowConfig& config) {
  //sdl init
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {

    logger_->log(LogLevel::Critical, std::format("Failed to Initialize SDL: {}", SDL_GetError()));
    return false;
  }


  // opengl config 
  _preInitOpenGLAttribs();
  // SDL2 window init
  windowWidth_   = config.width;
  windowHeight_  = config.height;
  windowTitle_   = config.title; 
  Uint32 evaluatedFlags = MapConfigToSDLFlags(config);


  // create window
  SDLWindow_.reset(SDL_CreateWindow(windowTitle_.c_str(), config.windowPositionX, config.windowPositionY, windowWidth_, windowHeight_, evaluatedFlags)); 

  if (!SDLWindow_) {
    logger_->log(LogLevel::Critical, std::format("Failed to create SDL Window: {}", SDL_GetError()));
    return false;

  }

  // Create OpenGL Context
  openGLSDLContext_.reset(SDL_GL_CreateContext(SDLWindow_.get()));
  if(!openGLSDLContext_){
    logger_->log(LogLevel::Critical, std::format("Failed to create SDL context: {}", SDL_GetError()));
    return false;
  }

  //bind window context to window 
  if (SDL_GL_MakeCurrent(SDLWindow_.get(), openGLSDLContext_.get()) < 0) {
    logger_->log(LogLevel::Critical, std::format("Failed to make current SDL window context: {}", SDL_GetError()));
    return false;
  }


  // glad loader!!
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    logger_->log(LogLevel::Critical, std::format("Failed to initialize GLAD {}", SDL_GetError()));
    return false; 
  }

  //vsync
  SDL_GL_SetSwapInterval(1);
  // if (SDL_GL_SetSwapInterval(-1) < 0) {
  //   SDL_GL_SetSwapInterval(1);
  // }

  //viewport 
  glViewport(0, 0, windowWidth_,windowHeight_);

  // validate log
  logger_->log(LogLevel::Info, std::format(
        "[Engine Info]\n"
        "- Vendor: {}\n"
        "- Renderer: {}\n"
        "- Version: {}\n"
        "- Shading Language: {}\n\n",
        reinterpret_cast<const char*>(glGetString(GL_VENDOR)),
        reinterpret_cast<const char*>(glGetString(GL_RENDERER)),
        reinterpret_cast<const char*>(glGetString(GL_VERSION)),
        reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));

  // the after stuff that needed GLAD
  _postInitOpenGLAttribs();
  return true;
}
// cleanup!
void OpenGLWindowServer::ShutdownWindow() { 


  openGLSDLContext_.reset(); 
  SDLWindow_.reset(); 
  SDL_Quit();
}
// trigger on event OpenGLWindowServer::quit
bool OpenGLWindowServer::ShouldClose(){
  // if (glContext) SDL_GL_DeleteContext(glContext);
  // if (SDLWindow_) SDL_DestroyWindow(SDLWindow_);
  ShutdownWindow();
  return true;
} 


// get 
int OpenGLWindowServer::GetWindowWidth() const{
  return windowWidth_;
};
int OpenGLWindowServer::GetWindowHeight() const{
  return windowHeight_;
};
bool OpenGLWindowServer::IsMinimized() const {
  Uint32 flags = SDL_GetWindowFlags(SDLWindow_.get());
  return (flags & SDL_WINDOW_MINIMIZED) != 0;
};
WindowMode OpenGLWindowServer::GetWindowMode() const {
  Uint32 flags = SDL_GetWindowFlags(SDLWindow_.get());
  if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) return WindowMode::BorderlessFullscreen;
  if (flags & SDL_WINDOW_FULLSCREEN)         return WindowMode::ExclusiveFullscreen;
  return WindowMode::Windowed;


};

// DI slop 
void* OpenGLWindowServer::GetNativeWindowHandle() const {
  return SDLWindow_.get();
}
void* OpenGLWindowServer::GetNativeContextRender() const {
  return openGLSDLContext_.get();
}
//adjustments?
void OpenGLWindowServer::SetWindowTitle(const std::string& title) {
  SDL_SetWindowTitle(SDLWindow_.get(), title.c_str());
}

void OpenGLWindowServer::SetWindowSize(int width, int height) {
  SDL_SetWindowSize(SDLWindow_.get(), width, height);
}

void OpenGLWindowServer::SetWindowMode(WindowMode mode) {
  Uint32 sdlFlag = 0;
  if (mode == WindowMode::ExclusiveFullscreen)           sdlFlag = SDL_WINDOW_FULLSCREEN;
  if (mode == WindowMode::BorderlessFullscreen) sdlFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;

  SDL_SetWindowFullscreen(SDLWindow_.get(), sdlFlag);
}

void OpenGLWindowServer::SetVSync(bool enabled) {
  // 1 = VSync, 0 = Immediate/Uncapped, -1 = Late Swap Tearing (Adaptive VSync)
  int interval = enabled ? 1 : 0;
  SDL_GL_SetSwapInterval(interval);
}

void OpenGLWindowServer::SetResizable(bool resizable) {
  SDL_SetWindowResizable(SDLWindow_.get(), resizable ? SDL_TRUE : SDL_FALSE);
}


// uh OpenGLWindowServer::oh.
bool OpenGLWindowServer::PollEvents() { 
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      // isRunning_ = false;
      ShouldClose(); 
      return false;
    }
    inputManager_->processEvents(event);

  }

  return true;
}


void OpenGLWindowServer::_preInitOpenGLAttribs(){
  // 1. version: we are on 4.6 CORE
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // 2. setup debugging functitonality 
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

  // buffer and colour stuff 
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

  // MSAA anti alsiasing?
  // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}
void OpenGLWindowServer::_postInitOpenGLAttribs(){
  // features needed, depth tests, culling, 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // antialaias - use it
  // glEnable(GL_MULTISAMPLE);

  // alpha blending 
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // use the advanced Debug Callback
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(_OpenGLDebugOutput, logger_);
}



namespace OpenGLDebugTool {

  // for i in range(5) | :s#\(GL_DEBUG_SEVERITY_\)\(\w*\)[,|$]#case \1\2: "Severity: \2"; break;\r# | endfor
  [[nodiscard]] constexpr std::string_view getSource(GLenum source) noexcept {
    switch (source) {
      case GL_DEBUG_SOURCE_API:             return "API";
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
      case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
      case GL_DEBUG_SOURCE_THIRD_PARTY:    return "THIRD_PARTY";
      case GL_DEBUG_SOURCE_APPLICATION:    return "APPLICATION";
      case GL_DEBUG_SOURCE_OTHER:          return "OTHER";
      default:                              return "UNKNOWN";
    }
  }

  [[nodiscard]] constexpr std::string_view getType(GLenum type) noexcept {
    switch (type) {
      case GL_DEBUG_TYPE_ERROR:               return "ERROR";
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
      case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
      case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
      case GL_DEBUG_TYPE_MARKER:              return "MARKER";
      case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
      case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
      case GL_DEBUG_TYPE_OTHER:               return "OTHER";
      default:                                return "UNKNOWN";
    }
  }

  // Maps OpenGL severity levels to your Logger's custom LogLevel
  [[nodiscard]] constexpr LogLevel getLogLevel(GLenum severity) noexcept {
    switch (severity) {
      case GL_DEBUG_SEVERITY_HIGH:         return LogLevel::Error; 
      case GL_DEBUG_SEVERITY_MEDIUM:       return LogLevel::Warning;
      case GL_DEBUG_SEVERITY_LOW:          return LogLevel::Info;
      case GL_DEBUG_SEVERITY_NOTIFICATION: return LogLevel::Misc;
      default:                             return LogLevel::Misc;
    }
  }
}
void APIENTRY OpenGLWindowServer::_OpenGLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam){
  if (id == 131169 || id == 131185 || id == 131204 || id == 131218) return;

  if (!userParam) return;
  auto* logger = static_cast<ILogger*>(const_cast<void*>(userParam));

  logger->log(
      OpenGLDebugTool::getLogLevel(severity),
      std::format("[OpenGL DEBUG] \n\tID: {}\n\tSource: {}\n\tType: {}\n\tMessage: {}", 
        id, 
        OpenGLDebugTool::getSource(source), 
        OpenGLDebugTool::getType(type), 
        std::string_view{message, static_cast<size_t>(length)})
      );
}


