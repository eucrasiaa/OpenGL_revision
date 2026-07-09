#pragma once

#include "IWindowServer.hpp"
#include <SDL_video.h>
#include <memory>
#include <glad/glad.h>


class IInputManager;
class ILogger;
class OpenGLWindowServer : public IWindowServer{


  protected:
    IInputManager* inputManager_ = nullptr;  
    ILogger* logger_ = nullptr; 

    struct SDLWindowDeleter {
      void operator()(SDL_Window* w) const { if (w) SDL_DestroyWindow(w); }
    };
    struct SDLGLContextDeleter {
      void operator()(void* ctx) const { if (ctx) SDL_GL_DeleteContext(ctx); }
    };
    std::unique_ptr<SDL_Window, SDLWindowDeleter> SDLWindow_ = nullptr;
    std::unique_ptr<void, SDLGLContextDeleter> openGLSDLContext_ = nullptr; 

    int windowWidth_=1280;
    int windowHeight_=720;
    std::string windowTitle_ = "Placeholder Title";

    
    void _preInitOpenGLAttribs();
    void _postInitOpenGLAttribs();


    // struct defined by opengl. yea its gross
    static void APIENTRY _OpenGLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

    //critical we use so 
    [[nodiscard]] static constexpr Uint32 MapConfigToSDLFlags(const WindowConfig& config) noexcept {
      // backend should be locked 
      Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN; 

      switch (config.mode) {
        case WindowMode::BorderlessFullscreen: flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; break;
        case WindowMode::ExclusiveFullscreen:  flags |= SDL_WINDOW_FULLSCREEN; break;
        case WindowMode::Windowed:             break;
      }

      if (config.resizable)      flags |= SDL_WINDOW_RESIZABLE;
      if (config.highDpi)        flags |= SDL_WINDOW_ALLOW_HIGHDPI;
      if (config.borderless)     flags |= SDL_WINDOW_BORDERLESS;
      if (config.startMaximized) flags |= SDL_WINDOW_MAXIMIZED;

      return flags;
    }
  public:


    explicit OpenGLWindowServer(IInputManager* inputManager, ILogger* logger) 
      : inputManager_(inputManager),
      logger_(logger){
        if (!inputManager_) { throw std::runtime_error("MnputManager cannot be null"); }
        if (!logger_) { throw std::runtime_error("Logger cannot be null"); }
      }
    //lifecycle
    // virtual ~OpenGLWindowServer() = default;
    // virtual bool InitializeWindow(const std::string& title, int width, int height, WindowMode mode);
    virtual bool InitializeWindow(const WindowConfig& config);
    // cleanup!
    virtual void ShutdownWindow();
    // trigger on event quit
    virtual bool ShouldClose(); 


    // get 
    virtual int GetWindowWidth() const; 
    virtual int GetWindowHeight() const;
    virtual bool IsMinimized() const;
    virtual WindowMode GetWindowMode() const;

    // DI slop 
    virtual void* GetNativeWindowHandle() const;
    virtual void* GetNativeContextRender() const;

    //adjustments?
    virtual void SetWindowTitle(const std::string& title);
    virtual void SetWindowSize(int width, int height);
    virtual void SetWindowMode(WindowMode mode);
    virtual void SetVSync(bool enabled);
    virtual void SetResizable(bool resizable);


    // uh oh.
    virtual bool PollEvents();

};
