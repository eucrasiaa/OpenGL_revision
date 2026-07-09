#pragma once
#include <string>
#include "WindowServerTypes.hpp"
class IWindowServer{ 
  public:

    //lifecycle
    virtual ~IWindowServer() = default;
    virtual bool InitializeWindow(const WindowConfig& config) = 0;
    // virtual bool InitializeWindow(const std::string& title, int width, int height, WindowMode mode) = 0;
    // cleanup!
    virtual void ShutdownWindow() = 0;
    // trigger on event quit
    virtual bool ShouldClose() =0; 


    // get 
    virtual int GetWindowWidth() const = 0;
    virtual int GetWindowHeight() const = 0;
    virtual bool IsMinimized() const = 0;
    virtual WindowMode GetWindowMode() const = 0;

    // DI slop 
    virtual void* GetNativeWindowHandle() const = 0;
    virtual void* GetNativeContextRender() const = 0;

    //adjustments?
    virtual void SetWindowTitle(const std::string& title) = 0;
    virtual void SetWindowSize(int width, int height) = 0;
    virtual void SetWindowMode(WindowMode mode) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual void SetResizable(bool resizable) = 0;


    // uh oh.
    // bool for should break game loop via quit
    virtual bool PollEvents() = 0;

};

