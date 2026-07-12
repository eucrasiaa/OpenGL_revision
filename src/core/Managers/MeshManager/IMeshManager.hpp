#pragma once
#include "MeshTypes.hpp" 
#include "VAOTypes.hpp"
#include <string>
class IMeshManager{

  public: 
    virtual ~IMeshManager() = default;
    virtual MeshHandle LoadMesh(const std::string& objPath)= 0;
    virtual void initalize(void * mystery) = 0; 
};
