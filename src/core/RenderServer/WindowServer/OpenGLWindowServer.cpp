#pragma once
#include "RenderServer/WindowServer/OpenGLWindowServer.hpp"
#include "InputManager/IInputManager.hpp"

#include <SDL2/SDL.h>
#include <SDL_video.h>

// Templated 
bool OpenGLWindowServer::InitializeWindow(const std::string& title, int width, int height, WindowMode mode) {
  SDLWindow_.reset(SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth_, windowHeight_, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
  openGLSDLContext_.reset(SDL_GL_CreateContext(SDLWindow_.get()));
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

// the OpenGLWindowServer::rest
