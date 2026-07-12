#pragma once
#include "MeshManager/IMeshManager.hpp"
#include "third_party/tiny_obj_loader.h"
class ILogger;
class MeshManager : public IMeshManager {
  private: 


    ILogger *logger_ = nullptr;
    
    struct MeshResource {
        MeshData data;
        uint16_t generation;
        uint16_t refCount;
        bool isFree;
    };
    static constexpr const int INDEX_MASK = 0x0000FFFF;
    static constexpr const int GEN_MASK = 0xFFFF0000;
    [[nodiscard]] static constexpr inline uint16_t getIndex(uint32_t val) noexcept {
      return static_cast<uint16_t>(val & INDEX_MASK);
    }

    [[nodiscard]] static constexpr inline uint16_t getGeneration(uint32_t val) noexcept {
      return static_cast<uint16_t>((val & GEN_MASK) >> 16);
    }

    std::vector<MeshResource> slots_;
    std::vector<uint16_t> freeSlots_; 
    std::unordered_map<std::string, MeshHandle> loadedMeshes_;


    GLuint vaoTemplate3D_ = 0;
    GLuint vaoTemplate3DBillboard_ = 0;
    GLuint vaoTemplate2D_ = 0;
    GLuint vaoTemplate3DUI_ = 0; 

    // the one quad shared by all!! 
    GLuint sharedQuadVBO_ = 0;
    GLuint sharedQuadEBO_ = 0;


    void initializeVAOTemplates();

    void createSharedQuadBuffers();
    void setupVertexAttributes3D(GLuint vao);
    void setupInstanceAttributes(GLuint vao);

    MeshHandle allocateHandle(const MeshData &toAlloc, const std::string& path = "");

  public:
    virtual MeshHandle loadMesh(const std::string& objPath) override;
    virtual void initalize(void * Logger) override; 
    virtual MeshData* getMesh(MeshHandle handle) override;
    virtual void unloadMesh(MeshHandle handle) override;
};

