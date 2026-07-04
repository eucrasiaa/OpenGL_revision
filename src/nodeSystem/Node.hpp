#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <glm/glm.hpp>
#include <RenderServer/RenderTypes.hpp>
class IRenderServer;

class Node {
  protected:

    // injected

    IRenderServer* renderServer_ = nullptr;  

    // glm::vec3 position = glm::vec3(0.0f);
    // glm::vec3 rotation = glm::vec3(0.0f); // x (pitch), y (yaw), z (roll) in degrees
    // glm::vec3 scale    = glm::vec3(1.0f);

    //transform matricies 
    glm::mat4 localTransform_ = glm::mat4(1.0f);
    glm::mat4 globalTransform_ = glm::mat4(1.0f);
    bool isDirty_ = true; // for initial sync math!  

    //children and parent
    std::vector<Node *> children_;
    Node* parent_=nullptr;


    GLuint64 textureHandle_ = 0;
    RenderLayer renderLayer_ = RenderLayer::Opaque;


    // Node(IRenderServer* renderServer) : renderServer_(renderServer) {
    //   if (!renderServer) {
    //     throw std::runtime_error("RenderServer cannot be null");
    //   }
    // }
  public:

    Node(IRenderServer* renderServer) : renderServer_(renderServer) {
      if (!renderServer) {
        throw std::runtime_error("RenderServer cannot be null");
      }
    }
    

    virtual ~Node() {
      for (auto* child : children_) {
        delete child;
      }
      children_.clear();
    }
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;


    // control transforms:

    void addChild(Node* child) {
      if (child) {
        child->parent_ = this;
        children_.push_back(child);
      }
    }

    void removeChild(Node* child) {
      auto it = std::find(children_.begin(), children_.end(), child);
      if (it != children_.end()) {
        (*it)->parent_ = nullptr;
        children_.erase(it);
      }
    }



    virtual void update(double dt){
      return;
    }
    

    // Transform methods
    void setLocalTransform(const glm::mat4& transform) {
      localTransform_ = transform;
      isDirty_ = true;
    }

    void setPosition(const glm::vec3& position) {
      localTransform_[3] = glm::vec4(position, 1.0f);
      isDirty_ = true;
    }

    void setRotation(const glm::quat& rotation) {
      //TODO

      isDirty_ = true;
    }

    void setScale(const glm::vec3& scale) {
      //TODO

      isDirty_ = true;
    }
    void markDirty() {
      isDirty_ = true;
    }


    void computeTransforms(const glm::mat4& parentTransform, bool parentIsDirty) {
        if (parentIsDirty || isDirty_) {
            globalTransform_ = parentTransform * localTransform_;
            isDirty_ = false;
        }
    }


    // render pass
    virtual void render(const glm::mat4& parentTransform, bool parentIsDirty) {
        computeTransforms(parentTransform, parentIsDirty);

        // RenderInstance instance;
        // instance.globalTransform = m_globalTransform;
        // instance.textureHandle = m_textureHandle;
        // instance.layer = static_cast<int>(m_renderLayer);
        // m_renderServer->SubmitInstance(instance);

        // Render children
        for (auto* child : children_) {
            child->render(globalTransform_, isDirty_);
        }

        isDirty_ = false;
    }



    void setTextureHandle(GLuint64 handle) { textureHandle_ = handle; }
    void setRenderLayer(RenderLayer layer) { renderLayer_ = layer; }

    const glm::mat4& getGlobalTransform() const { return globalTransform_; }
    const std::vector<Node*>& getChildren() const { return children_; }
    Node* getParent() const { return parent_; }


    // virtual ~Node();
    // // virtual void render(const glm::mat4 &parentTransform);
    // virtual void addChild(Node *node);
    //
    // virtual void render(const glm::mat4& parentGlobal, bool parentIsDirty);
    // void triggerCompute();
    // virtual void computeTransforms(const glm::mat4& parentGlobal, bool parentIsDirty);
};
