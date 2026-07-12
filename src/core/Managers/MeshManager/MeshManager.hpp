#pragma once
#include "MeshManager/IMeshManager.hpp"
class MeshManager : public IMeshManager { 

    virtual MeshHandle LoadMesh(const std::string& objPath) override;
    virtual void initalize(void * Logger) override; 
};
