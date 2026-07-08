#pragma once
#include <SDL_video.h>
#include <sdl_video.h>
#include <string>
enum class WindowMode {
  Windowed,
  BorderlessFullscreen,
  ExclusiveFullscreen, 
};

struct WindowConfig {
    std::string title   = "Placeholder Title-WillEngine Window";
    int width           = 1280;
    int height          = 720;
    WindowMode mode     = WindowMode::Windowed;
    bool resizable      = false;
    bool highDpi        = false;
    bool borderless     = false;
    bool startMaximized = false;
    int windowPositionX = SDL_WINDOWPOS_CENTERED;
    int windowPositionY = SDL_WINDOWPOS_CENTERED;
};

// struct bonusSettings{ 
//   int positionX = SDL_WINDOWPOS_CENTERED;
//   int positionY = SDL_WINDOWPOS_CENTERED;
//
// }bonusSettings;
