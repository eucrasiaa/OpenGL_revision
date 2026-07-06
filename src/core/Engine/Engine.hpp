#pragma once
#include "Engine/IEngine.hpp"
// #include <glad/glad.h>
// #include <SDL2/SDL.h>
// #include <glm/trigonometric.hpp>
#include <vector>
#include <memory>

#ifdef PERFORMANCE 
#include <print>
#include "ansi_min.hpp"
#endif

class Node;
class IRenderServer;
class IInputManager;
class ILogger;

class Engine : public IEngine{
  private:

    //injected OBSERVERS (uses, doesnt own)
    IRenderServer* renderServer_ = nullptr;
    IInputManager* inputManager_ = nullptr; 

    ILogger* logger_ = nullptr; 
    //owned 
    std::vector<Node*> sceneNodes_; // change ptr type maybe 
                                    // std::vector<std::unique_ptr<Node>> sceneNodes_; 


                                    //privs
    void handleEvents();
    void update(double dt);
    void render(double dt);


    //game state
    bool isRunning_ = false;
    double timeScale_ = 1.0;
    double deltaTime_ = 0.0;

    // Engine() = default;
    // ~Engine() { shutdown(); }
    // Engine(const Engine&) = delete;
    // Engine& operator=(const Engine&) = delete;
#ifdef PERFORMANCE 
    inline void print_engine_perf(double fps, double total_ms, double update_ms, double recalc_ms,double render_ms, double events_ms);
#endif 

  public:

    explicit Engine(IRenderServer* renderServer, IInputManager* inputManager, ILogger* logger) 
      : renderServer_(renderServer),
      inputManager_(inputManager),
        logger_(logger){
        if (!renderServer_) { throw std::runtime_error("RenderServer cannot be null"); }
        if (!inputManager_) { throw std::runtime_error("MnputManager cannot be null"); }
        if (!logger_) { throw std::runtime_error("Logger cannot be null"); }
      }
    // explicit Engine(IRenderServer* renderServer) 
    //   : renderServer_(renderServer) {
    //     if (!renderServer_) {
    //       throw std::runtime_error("RenderServer cannot be null");
    //     }
    //   }
    
    ~Engine() { shutdown(); }

    bool init();
    void run();
    void shutdown();
    void addNode(Node* node);
    void removeNode(Node* node);

    bool isRunning() const { return isRunning_; }
    double getDeltaTime() const { return deltaTime_; }
    IRenderServer* getRenderServer() { return renderServer_; }
    // float yaw   = glm::radians(-90.0f); 
    // float pitch = 0.0f;
    // float roll = 0.0f;
    // float mouseSensitivity = glm::radians(0.1f);
    // float movementSpeed    = 0.2f; 
    // void MoveCamera();
};
