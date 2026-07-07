#pragma once
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glad/glad.h>
// #include <SDL2/SDL.h>
// #include <iostream>
// #include <string>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include "IRenderServer.hpp"
#include <glad/glad.h>
#include <SDL_video.h>
#include <string>
#include <unordered_map>
// #include <memory>

class ILogger;
class IInterpolateServer;
class RenderServer : public IRenderServer { 
  protected:

    IInterpolateServer* interpolateServer_ = nullptr;

    // optinal deptendency
    ILogger* logger_ = nullptr;
    // built on scene change? 
    std::vector<RenderEntity> renderEntities_;




   void resolveWorldTransform(EntityID id, double alpha);

  public:
    explicit RenderServer(IWindowServer* windowServer, IInterpolateServer *interpolateServer, ILogger* logger = nullptr) 
      : IRenderServer(windowServer), // Call base constructor here
      interpolateServer_(interpolateServer),
      logger_(logger) {

        if (!windowServer_) { throw std::runtime_error("RenderServer's windowServer cannot be null"); }
        if (!interpolateServer_) { throw std::runtime_error("renderserver's interpolation server cant be null!!!!");}
        if (!logger_) { throw std::runtime_error("RenderServer's windowServer cannot be null"); }

      }
    ~RenderServer() { shutdown(); }
    // RenderServer(const RenderServer&) = delete;
    // RenderServer& operator=(const RenderServer&) = delete;
    // RenderServer(RenderServer&&) = delete;
    // RenderServer& operator=(RenderServer&&) = delete;

    virtual bool init(const char* title, int width, int height) override;
    virtual void shutdown() override;
    virtual void render(double alpha, double elapsedTime) override;
    virtual bool pollServer() override;

    virtual void resolveAllTransforms(double alpha) override;
};


// class RenderServer : public IRenderServer {
//   private:
//     // optinal deptendency
//     ILogger* logger_ = nullptr;
//     // |||||  SDL2 Items  |||||
//     SDL_Window* window_ =nullptr;
//     SDL_GLContext glContext_ = nullptr;
//
//     // ||||| OpenGL Items |||||
//     // |||||||||||||||||||||||| 
//     // VAO
//     GLuint gVertexArrayObject_ = 0;
//     // VBO
//     GLuint gVertexBufferObject_ = 0;
//     // IBO
//     GLuint gIndexBufferObject_ = 0;  
//     // Instance VBO 
//     GLuint gInstanceVBO_ = 0;
//     // pipeline
//     // std::unordered_map<std::string, Pipeline> gPipelinePrograms{};
//
//
//     // framebuff for world stuff
//     GLuint worldFBO_ = 0;
//     GLuint worldColorTex_ = 0;
//     GLuint worldDepthStencilRBO_ = 0;
//     // framebuff for full screen postProcessing
//     GLuint postFBO_ = 0;
//     GLuint postColorTex_ = 0;
//
//     //Samplers
//     GLuint linearSampler_ = 0;
//     GLuint nearestSampler_ = 0;
//     bool useLinearFilter_ = false;
//
//     //configs
//     bool only2D_ = true;
//     bool doWPP_ = true;
//     bool doSPP_ = false;
//     std::string PostProcessMode_ = "WorldPostProcess";
//
//     int width_ = 0;
//     int height_ = 0;
//
//     //camera
//     glm::vec3 cameraPos_ = glm::vec3(0.0f, 0.0f, 100.0f); 
//     glm::vec3 cameraLook_ = glm::vec3(0.0f, 0.0f, 0.0f); 
//     glm::vec3 cameraUp_ = glm::vec3(0.0f, 1.0f, 0.0f);
//
//     // ||||| instange management |||||
//     // ||||||||||||||||||||||||||||||| 
//     // buckets rework
//     // std::vector<RenderInstance> opaqueQueue;
//     // std::vector<RenderInstance> transparentQueue;
//     // std::vector<RenderInstance> ui_3d_Queue;
//     // std::vector<RenderInstance> ui_2d_Queue;
//     // std::unordered_map<RenderItemID, RenderInstance*> instanceDirectory;
//     // RenderItemID NextID = 0;    
//
//
//     // ||||| Function!!! ||||
//     // |||||||||||||||||||||
//
//     // init 
//     void VertexSpecification();
//     void FrameBufferInit();
//     void InitPipelines();
//     void InitSamplers();
//     // init helpers
//     // void CreateGraphicsPipeline(const PipelineConfig& config);
//     int BindPipeline(const std::string& name);
//     std::string LoadShaderSource(const std::string& filePath);
//     GLuint CompileShader(GLuint type, const std::string& source);
//     GLenum GetShaderTypeFromExtension(const std::string& filePath);
//
//
//     // render stuff! mostly helpers
//     void DrawFullScreenQuad();
//     // void RenderQueue(const std::vector<RenderInstance>& queue);
//     void FlushInstancedBindlessBatch(GLuint vaoID, GLuint indexCount, const std::vector<glm::mat4>& matrices, const std::vector<GLuint64>& handles);
//     void FlushInstancedBatch(GLuint vaoID, GLuint textureID, GLuint indexCount, const std::vector<glm::mat4>& matrices);
//     void FlushInstancedBatch2d(GLuint textureID, const std::vector<glm::mat4>& matrices);
//
//
//     // void setProjectionUniform(GLuint programID, RenderPassType passType=RenderPassType::World);
//
//
//     static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
//                           GLsizei length, const char *message, const void *userParam);
//
//
//   public:
//
//     explicit RenderServer(ILogger* logger = nullptr) 
//         : logger_(logger) {}
//
//     ~RenderServer() { shutdown(); }
//
//
//     RenderServer(const RenderServer&) = delete;
//     RenderServer& operator=(const RenderServer&) = delete;
//     RenderServer(RenderServer&&) = delete;
//     RenderServer& operator=(RenderServer&&) = delete;
//
//
//     bool init(const char* title, int width, int height) override;
//     void shutdown() override;
//     void render(double dt) override;
//     void ClearQueues() override;
//     void setCamera(const glm::vec3& pos, const glm::vec3& look, const glm::vec3& up) override;
//     void setPostProcessMode(const std::string& mode) { PostProcessMode_ = mode; }
//
//     //regestering
//     // void SubmitInstance(const RenderInstance& instance) override;
//
//
//     //get set
//     int getWidth() const { return width_; }
//     int getHeight() const { return height_; }
//
// };
