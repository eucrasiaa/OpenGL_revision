#pragma once
#include <functional>
#include <memory>

class Node;
#include "SceneCommand/ISceneCommand.hpp"



class Scene {
private:
    friend class Engine;
    friend struct wEngApplication;
    static inline ISceneCommand* sCommands_ = nullptr;

public:
    static void spawn(Node* parent, std::function<std::unique_ptr<Node>()> factory) {
        if (sCommands_) sCommands_->requestSpawn(parent, factory);
    }
    static void destroy(Node* node) {
        if (sCommands_) sCommands_->requestDestroy(node);
    }
};
