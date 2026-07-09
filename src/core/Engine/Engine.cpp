#include "Engine.hpp"
#include "Node.hpp"
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_timer.h>

#include "Logger/ILogger.hpp"
#include "RenderServer/IRenderServer.hpp"
#include "InputManager/IInputManager.hpp"

#include "SceneManager/ISceneManager.hpp"


// #include <glad/glad.h>
// #include "RenderServer.hpp"
// bool Engine::init(const char* title, int width, int height) {
bool Engine::init() {
  isRunning_ = true;
  return true;
}

// #define PERFORMANCE

void Engine::run() {
#ifdef PERFORMANCE
  double PERFORMANCE_fpsTimer = 0.0;
  int PERFORMANCE_frameCount = 0;
  uint64_t PERFORMANCE_totalUpdateTicks = 0;
  uint64_t PERFORMANCE_totalRecalcTicks = 0;
  uint64_t PERFORMANCE_totalRenderTicks = 0;
  uint64_t PERFORMANCE_totalEventsTicks = 0;
#endif

  const double FIXED_TIMESTEP = 1.0 / 60.0; 
  const double MAX_DELTA = 0.25;
  double accumulator = 0.0;
  double elapsedTime = 0.0;
  double alpha_interp =  0.0; //accumulator / FIXED_TIMESTEP; 
  uint64_t frequency = SDL_GetPerformanceFrequency();
  const double INV_FREQUENCY = 1.0 / static_cast<double>(SDL_GetPerformanceFrequency());

// thread

  uint64_t currentTicks = SDL_GetPerformanceCounter(); // keep this right before while!
  while (isRunning_) {
    uint64_t newTicks = SDL_GetPerformanceCounter();

#ifdef PERFORMANCE
    double PERFORMANCE_rawFrameTime = static_cast<double>(newTicks - currentTicks) / frequency;
#endif

    //double frameTime = static_cast<double>(newTicks - currentTicks) / frequency;
    double frameTime = static_cast<double>(newTicks - currentTicks) * INV_FREQUENCY;
    currentTicks = newTicks;
    if (frameTime > MAX_DELTA) {
      frameTime = MAX_DELTA;
    }
    elapsedTime += (frameTime * timeScale_);
    accumulator += (frameTime * timeScale_);


#ifdef PERFORMANCE
    uint64_t startEvents = SDL_GetPerformanceCounter();
#endif
    handleEvents();

#ifdef PERFORMANCE
    uint64_t endEvents = SDL_GetPerformanceCounter();
    PERFORMANCE_totalEventsTicks += (endEvents - startEvents);
#endif 

#ifdef PERFORMANCE
    uint64_t startUpdate = SDL_GetPerformanceCounter();
#endif


    while (accumulator >= FIXED_TIMESTEP) {
      // runGameLogic(FIXED_TIMESTEP);
      sceneManager_->update(FIXED_TIMESTEP);
      sceneManager_->runEngineLogic(FIXED_TIMESTEP);
      // runEngineLogic(FIXED_TIMESTEP);
      accumulator -= FIXED_TIMESTEP;
    }
#ifdef PERFORMANCE
    uint64_t endUpdate = SDL_GetPerformanceCounter();
    PERFORMANCE_totalUpdateTicks += (endUpdate - startUpdate);
#endif
    sceneManager_->flushCommands();

#ifdef PERFORMANCE
    uint64_t startRecalc = SDL_GetPerformanceCounter();
#endif

    // recalculate matric in prep for interpolate
    // recalculateNodes(elapsedTime);
    sceneManager_->recalculateTransforms();


#ifdef PERFORMANCE
    uint64_t endRecalc= SDL_GetPerformanceCounter();
    PERFORMANCE_totalRecalcTicks += (endRecalc - startRecalc);
#endif 
    


    // renderServer_->advanceInterpolate();



#ifdef PERFORMANCE
    uint64_t startRender = SDL_GetPerformanceCounter();
#endif 

    // passes off to renderserver to draw 
    // thats interpolate calc, and use that to draw 
    renderServer_->render(alpha_interp, elapsedTime);

#ifdef PERFORMANCE 
    uint64_t endRender = SDL_GetPerformanceCounter();
    PERFORMANCE_totalRenderTicks += (endRender - startRender);
    PERFORMANCE_frameCount++;
    PERFORMANCE_fpsTimer += PERFORMANCE_rawFrameTime;
    if (PERFORMANCE_fpsTimer >= 1.0) {
      double avgFrameTimeMs = (PERFORMANCE_fpsTimer / PERFORMANCE_frameCount) * 1000.0;
      double currentFps = static_cast<double>(PERFORMANCE_frameCount) / PERFORMANCE_fpsTimer;
      double avgUpdateMs = (static_cast<double>(PERFORMANCE_totalUpdateTicks) / PERFORMANCE_frameCount / frequency) * 1000.0;
      double avgRecalcMs = (static_cast<double>(PERFORMANCE_totalRecalcTicks) / PERFORMANCE_frameCount / frequency) * 1000.0;
      double avgRenderMs = (static_cast<double>(PERFORMANCE_totalRenderTicks) / PERFORMANCE_frameCount / frequency) * 1000.0;
      double avgEventsMs = (static_cast<double>(PERFORMANCE_totalEventsTicks) / PERFORMANCE_frameCount / frequency) * 1000.0;
      print_engine_perf(currentFps, avgFrameTimeMs, avgUpdateMs,avgRecalcMs, avgRenderMs, avgEventsMs);
      // reset intervals
      PERFORMANCE_frameCount = 0;
      PERFORMANCE_fpsTimer = 0.0;
      PERFORMANCE_totalUpdateTicks = 0;
      PERFORMANCE_totalRecalcTicks = 0;
      PERFORMANCE_totalRenderTicks = 0;
      PERFORMANCE_totalEventsTicks =0;
    }
#endif
  
    // Write back input status 
    inputManager_->updateState();

  }
}
void Engine::addNode(Node* node) {
  // std::print(" From Engine: {:#x}\n", reinterpret_cast<std::uintptr_t>(node));
  if (node != nullptr) {
    std::unique_ptr<Node> toAdd;
    toAdd.reset(node);
    sceneManager_->addNodeTree(std::move(toAdd));
    // sceneNodes_.push_back(node);
    // TODO pass off to scenemanager
  }
}

void Engine::shutdown() {
  sceneManager_->shutdown();
  renderServer_->shutdown();
}



// void Engine::runGameLogic(double dt) {
//   for (auto *node : sceneNodes_){
//     if (node != nullptr) { 
//       node->update(dt);
//     }
//   }
// }
//
//
// void Engine::runEngineLogic(double dt) {
//   //TODO calls to physics, etc
//   return;
// }
//
// void Engine::recalculateNodes(double ft)=0;{
//   for (auto* node : sceneNodes_) {
//     if (node != nullptr) { 
//       node->_checkCalculate();
//     }
//   } 
// }

void Engine::handleEvents() {
 // CALL THE WINDOW POLL -ER 
 // false return = exit
 if (!renderServer_->pollServer()){
   isRunning_=false;
 }
    //
    // else if (event.type == SDL_KEYDOWN) {
    //   switch (event.key.keysym.sym) {
    //     case SDLK_LEFT:
    //       break;
    //     case SDLK_RIGHT:
    //       break;
    //     case SDLK_SPACE:
    //       // SDL_SetRelativeMouseMode(inputState.ToggleMouse? SDL_TRUE : SDL_FALSE);
    //       break;
    //     case SDLK_COMMA:
    //       break;
    //     case SDLK_PERIOD:
    //       break;
    //
    //   }
    // }
    // if(event.type == SDL_MOUSEMOTION) {
    // yaw   += event.motion.xrel * mouseSensitivity;
    // pitch -= event.motion.yrel * mouseSensitivity; 
    // float maxPitch = glm::radians(89.0f);
    // if (pitch > maxPitch)  pitch = maxPitch;
    // if (pitch < -maxPitch) pitch = -maxPitch;
    // }

  
}


#ifdef PERFORMANCE 

inline void Engine::print_engine_perf(double fps, double total_ms, double update_ms, double recalc_ms, double render_ms, double events_ms){
  // std::print("{}", term::mv::CURSOR_HOME);
  term::move_up(11,true);
  auto performance_color = (total_ms > 16.67) ? term::fg::B_RED : term::fg::B_GREEN;
  double budget_percent = (total_ms / 16.67) * 100.0;
  std::print("\033[2K{}{}{}┌────────────────────────────────────────┐\n", term::format::BOLD, term::fg::B_CYAN, term::format::RESET);
  std::print("\033[2K{}{}│ {}{}STATS:{}│\n", term::format::BOLD, term::fg::B_CYAN, term::fg::B_YELLOW, term::format::BOLD, " ");
  std::print("\033[2K{}{}├────────────────────────────────────────┤\n{}", term::format::BOLD, term::fg::B_CYAN, term::format::RESET);
  std::print("\033[2K{}{}│ {}Frame Rate:  {}{:>6.2f} FPS{} │\n", 
      term::format::BOLD, term::fg::B_CYAN, term::fg::WHITE, performance_color, fps, term::format::RESET);
  std::print("\033[2K{}{}│ {}Frame Budget: {}{:>6.2f} ms {}({:>5.1f}%%){} │\n", 
      term::format::BOLD, term::fg::B_CYAN, term::fg::WHITE, performance_color, total_ms, term::format::DIM, budget_percent, term::format::RESET);
  std::print("\033[2K{}{}├────────────────────────────────────────┤\n{}", term::format::BOLD, term::fg::B_CYAN, term::format::RESET);
  std::print("\033[2K{}{}│ {} -> {}Core Update: {:>6.10f} ms{} │\n", 
      term::format::BOLD, term::fg::B_CYAN, term::format::RESET, term::fg::B_BLUE, update_ms, term::format::RESET);
  std::print("\033[2K{}{}│ {} -> {}GPU Render:  {:>6.10f} ms{} │\n", 
      term::format::BOLD, term::fg::B_CYAN, term::format::RESET, term::fg::B_MAGENTA, render_ms, term::format::RESET);

  std::print("\033[2K{}{}│ {} -> {}Calc Update: {:>6.10f} ms{} │\n", 
      term::format::BOLD, term::fg::B_CYAN, term::format::RESET, term::fg::B_GREEN, recalc_ms, term::format::RESET);
  std::print("\033[2K{}{}│ {} -> {}Event Update:{:>6.10f} ms{} │\n", 
      term::format::BOLD, term::fg::B_CYAN, term::format::RESET, term::fg::B_GREEN, events_ms, term::format::RESET);
  std::print("\033[2K{}{}└────────────────────────────────────────┘\n{}", term::format::BOLD, term::fg::B_CYAN, term::format::RESET);
  fflush(stdout);
}

#endif
