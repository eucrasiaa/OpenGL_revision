#pragma once
class Node;
class IEngine {
public:
    virtual ~IEngine() = default;
    virtual bool init() = 0;
    virtual void run() = 0;
    virtual void shutdown() = 0;
    virtual void addNode(Node* node) = 0;
};
