#pragma once
// #include <glad/glad.h>
// #include <SDL2/SDL.h>
// #include <glm/trigonometric.hpp>
#include <vector>
#include <memory>


class Node;
class IRenderServer;
class IInputManager;

// class AlphaEngine {
//   private:
//
//     //injected
//     IRenderServer* renderServer_ = nullptr;
//     IInputManager* inputManager_ = nullptr; 
//
//     //owned 
//     std::vector<Node*> sceneNodes; // change ptr type maybe 
//
//
//     //privs
//     void handleEvents();
//     void update(double dt);
//     void render(double dt);
//
//
//     //game state
//     bool isRunning_ = false;
//     double timeScale_ = 1.0;
//     double deltaTime_ = 0.0;
//
//     // Engine() = default;
//     // ~Engine() { shutdown(); }
//     // Engine(const Engine&) = delete;
//     // Engine& operator=(const Engine&) = delete;
//
//
//   public:
//
//     explicit Engine(IRenderServer* renderServer) 
//       : renderServer_(renderServer) {
//         if (!renderServer) {
//           throw std::runtime_error("RenderServer cannot be null");
//         }
//       }
//
//     ~Engine() { shutdown(); }
//
//     bool init();
//     void run();
//     void shutdown();
//     void addNode(Node* node);
//     void removeNode(Node* node);
//
//     bool isRunning() const { return isRunning_; }
//     double getDeltaTime() const { return deltaTime_; }
//     IRenderServer* getRenderServer() { return renderServer_; }
//     // float yaw   = glm::radians(-90.0f); 
//     // float pitch = 0.0f;
//     // float roll = 0.0f;
//     // float mouseSensitivity = glm::radians(0.1f);
//     // float movementSpeed    = 0.2f; 
//     // void MoveCamera();
// };
