#pragma once



// holds master draw list
// holds master material list
// does entity regestration  registerEntity(EntityID, MeshHandle, Material
//
// initalizes the material SSBO


#include "RenderUtils/DrawCommand.hpp"
#include "RenderUtils/RenderTypes.hpp"
class IMeshManager;
class IMaterialManager;
class IInterpolateServer;
class IRenderScene{

    IRenderScene(IMeshManager* meshMgr, IMaterialManager* matMgr);
    
};
