#pragma once
#include "Node.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <RenderServer/RenderTypes.hpp>

class SpacialNode : public Node {

  private:
  protected:
  public:
    using Node::Node;
    // user facing easy
    //  TRUE STATE!!
    glm::vec3 position_ = glm::vec3(0.0f);
    // psudo exists, changes to this are actually mapped to the quaterion for math sake?
    glm::vec3 rotation_ = glm::vec3(0.0f); // x (pitch), y (yaw), z (roll) in degrees
    glm::vec3 scale_    = glm::vec3(1.0f);

    glm::quat quaternion_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // QUAT!!

    // CACHED FOR MATH 
    //transform matricies 
    glm::mat4 localTransform_ = glm::mat4(1.0f);
    glm::mat4 globalTransform_ = glm::mat4(1.0f);

    // "Basis"
    glm::vec3 forward_ = glm::vec3(0.0f, 0.0f, -1.0f); 
    glm::vec3 up_      = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right_   = glm::vec3(1.0f, 0.0f, 0.0f);
    bool isDirty_ = true; // for initial sync math!  


    GLuint64 textureHandle_ = 0;
    RenderLayer renderLayer_ = RenderLayer::Opaque;


    // math stuff 
    virtual void checkCalculate(const glm::mat4& parentTransform, bool parentIsDirty) override;

    void computeTransforms(const glm::mat4& parentTransform);


    // Transform methods
    void setLocalTransform(const glm::mat4& transform);

    void setPosition(const glm::vec3& position);

    void setRotation(const glm::quat& rotation);

    void setScale(const glm::vec3& scale);
    void markDirty();

    void setTextureHandle(GLuint64 handle) { textureHandle_ = handle; }
    void setRenderLayer(RenderLayer layer) { renderLayer_ = layer; }

    const glm::mat4& getGlobalTransform() const { return globalTransform_; }

    virtual void printInfo(bool recurse = false, int indent=0) override;


};
