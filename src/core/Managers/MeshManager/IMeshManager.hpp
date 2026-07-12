#pragma once
#include "MeshTypes.hpp" 
#include "VAOTypes.hpp"
#include <string>
class IMeshManager{

  public: 
    virtual ~IMeshManager() = default;
    virtual MeshHandle loadMesh(const std::string& objPath)= 0;
    virtual void initalize(void * mystery) = 0; 
    virtual MeshData* getMesh(MeshHandle handle) =0;

    virtual void unloadMesh(MeshHandle handle) =0;
};
