#include "Engine.hpp"
#include "RenderServer/RenderServer.hpp"
#include "Node.hpp"

#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_timer.h>

// #include <glad/glad.h>

// #include "RenderServer.hpp"
//
//
// // bool Engine::init(const char* title, int width, int height) {
// bool Engine::init() {
//   // if(!RenderServer::Get().init(title, width, height)){
//     // std::cerr<<"render server failed to initialize!\n";
//     // return false;
//   // }
//
//   isRunning_ = true;
//   return true;
// }
//
//
// void Engine::update(double dt) {
//   for (auto *node : sceneNodes){
//
//     node->update(dt);
//   }
//   // if (sceneNodes.size() > 1) {
//   //   Node* player = sceneNodes[1];
//   //
//   //   float speed = 150.0f;
//   //   player->position.x += speed * static_cast<float>(dt);
//   // }
// }
//
//
// void Engine::run() {
//   double accumulator = 0.0;
//   const double FIXED_DT = 1.0 / 60.0; 
//   const double MAX_DELTA = 0.25;
// #ifdef PERFORMANCE
//   double fpsTimer = 0.0;
//   int frameCount = 0;
//   uint64_t totalUpdateTicks = 0;
//   uint64_t totalRenderTicks = 0;
// #endif
//   double elapsedTime = 0.0;
//   uint64_t frequency = SDL_GetPerformanceFrequency();
//   uint64_t currentTicks = SDL_GetPerformanceCounter();
//   while (isRunning_) {
//     uint64_t newTicks = SDL_GetPerformanceCounter();
//
// #ifdef PERFORMANCE
//     double rawFrameTime = static_cast<double>(newTicks - currentTicks) / frequency;
// #endif
//     double frameTime = static_cast<double>(newTicks - currentTicks) / frequency;
//     currentTicks = newTicks;
//
//     if (frameTime > MAX_DELTA) {
//       frameTime = MAX_DELTA;
//     }
//     elapsedTime += (frameTime * timeScale_);
//     accumulator += (frameTime * timeScale_);
//
//     handleEvents();
// #ifdef PERFORMANCE
//     uint64_t startUpdate = SDL_GetPerformanceCounter();
// #endif
//     while (accumulator >= FIXED_DT) {
//       update(FIXED_DT);
//       accumulator -= FIXED_DT;
//     }
// #ifdef PERFORMANCE
//     uint64_t endUpdate = SDL_GetPerformanceCounter();
//     totalUpdateTicks += (endUpdate - startUpdate);
//     uint64_t startRender = SDL_GetPerformanceCounter();
// #endif
//     render(elapsedTime); // loads renderServer if needed
//     // Get::Renderer().render(elapsedTime);
//     renderServer_->render(elapsedTime);
//     // RenderServer::Get().render(elapsedTime);
// #ifdef DEBUG_WINDOW
//     RenderServer::Get().DrawDebug();
// #endif
//
// #ifdef PERFORMANCE 
//     uint64_t endRender = SDL_GetPerformanceCounter();
//     totalRenderTicks += (endRender - startRender);
//     frameCount++;
//     fpsTimer += rawFrameTime;
//     if (fpsTimer >= 1.0) {
//       double avgFrameTimeMs = (fpsTimer / frameCount) * 1000.0;
//       double currentFps = static_cast<double>(frameCount) / fpsTimer;
//
//       double avgUpdateMs = (static_cast<double>(totalUpdateTicks) / frameCount / frequency) * 1000.0;
//       double avgRenderMs = (static_cast<double>(totalRenderTicks) / frameCount / frequency) * 1000.0;
//
//       term::print_engine_perf(currentFps, avgFrameTimeMs, avgUpdateMs, avgRenderMs);
//
//       // Reset intervals
//       frameCount = 0;
//       fpsTimer = 0.0;
//       totalUpdateTicks = 0;
//       totalRenderTicks = 0;
//     }
// #endif
//   }
// }
// void Engine::addNode(Node* node) {
//   if (node != nullptr) {
//     sceneNodes.push_back(node);
//   }
// }
//
// void Engine::shutdown() {
//   for (auto* node : sceneNodes) {
//     delete node;
//   }
//   sceneNodes.clear();
//   renderServer_->shutdown();
// }
//
//
// void Engine::render(double ft) {
//   for (auto* node : sceneNodes) {
//     if (node != nullptr) { 
//       continue;
//     }
//   } 
// }
//
// void Engine::handleEvents() {
//   SDL_Event event;
//   while (SDL_PollEvent(&event) != 0) {
//     if (event.type == SDL_QUIT) {
//       isRunning_ = false;
//       shutdown();
//     }
//     else if (event.type == SDL_KEYDOWN) {
//       switch (event.key.keysym.sym) {
//         case SDLK_LEFT:
//           break;
//         case SDLK_RIGHT:
//           break;
//         case SDLK_SPACE:
//           // SDL_SetRelativeMouseMode(inputState.ToggleMouse? SDL_TRUE : SDL_FALSE);
//           break;
//         case SDLK_COMMA:
//           break;
//         case SDLK_PERIOD:
//           break;
//
//       }
//     }
//     // if(event.type == SDL_MOUSEMOTION) {
//     // yaw   += event.motion.xrel * mouseSensitivity;
//     // pitch -= event.motion.yrel * mouseSensitivity; 
//     // float maxPitch = glm::radians(89.0f);
//     // if (pitch > maxPitch)  pitch = maxPitch;
//     // if (pitch < -maxPitch) pitch = -maxPitch;
//     // }
//
//   }
// }
//
