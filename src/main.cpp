#include "TestingNode.hpp"
#if defined(_WIN32)
// #define NOMINMAX
// #define WIN32_LEAN_AND_MEAN
// #include <windows.h>
#include <libloaderapi.h>
// #undef OPAQUE 
// #undef TRANSPARENT
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h> 
#endif

#define SDL_MAIN_HANDLED
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include <stdbool.h>
// #include "GL_HELP.hpp"

#include "RenderServer/RenderServer.hpp"
#include "wEngApplication.hpp"
#include "Engine/Engine.hpp"
#include "nodeSystem/Node.hpp"
#include "nodeSystem/SpacialNode.hpp"



#include <filesystem>

static std::filesystem::path EXECUTABLE_DIR;

int main(int argc, char* argv[]) {

#if defined(_WIN32)
  wchar_t buffer[MAX_PATH];
  DWORD length = GetModuleFileNameW(NULL, buffer, MAX_PATH);
  if (length == 0) {
    throw std::runtime_error("Windows: Failed to retrieve executable path.");
  }
  std::wcout<<"Windows: "<<buffer<<std::endl;
  EXECUTABLE_DIR = std::filesystem::path(buffer).parent_path();
#elif defined(__linux__)
  char buffer[PATH_MAX];
  ssize_t length = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (length == -1) {
    throw std::runtime_error("Linux: Failed to read executable symlink.");
  }
  EXECUTABLE_DIR = std::filesystem::path(std::string(buffer)).parent_path();
#elif defined(__APPLE__)
  uint32_t size = 0;
  _NSGetExecutablePath(nullptr, &size); 
  char buffer[size];
  if (_NSGetExecutablePath(buffer, &size) != 0) {
    throw std::runtime_error("macOS: Failed to retrieve executable path.");
  }
  EXECUTABLE_DIR = std::filesystem::weakly_canonical(std::filesystem::path(buffer)).parent_path();
#endif



  SDL_SetMainReady(); 
  try {
    // RenderServer renderServer(&logger);
    // if (!renderServer.init("OpenGL Revision", 1280, 720)) {
    //   std::cerr << "Failed to initialize render server" << std::endl;
    //   return 1;
    // }
    //
    // Engine engine(&renderServer);
    wEngApplication wApp;
    
    auto rootNode = new SpacialNode();
    auto childNode = new SpacialNode();

    rootNode->addChild(childNode);
    childNode->setPosition(glm::vec3(10.0f, 5.0f, 0.0f));
    // childNode->setTextureHandle(0);
    std::print(" From Main: {:#x}\n", reinterpret_cast<std::uintptr_t>(rootNode));
   
    //
    // auto sn = new spawnerNode();
    // auto sn2 = new spawnerNode();
    // auto sn3 = new spawnerNode();
    wApp.engine_->addNode(rootNode);

    wApp.engine_->init();

    wApp.engine_->run();



  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }
  
  // if (!Engine::Get().init("OpenGL Engine Layout", 1280, 720)) {
  //   return 1;
  // }
  //
  //
  // Engine::Get().run();
  // Engine::Get().shutdown();
  return 0;
}
