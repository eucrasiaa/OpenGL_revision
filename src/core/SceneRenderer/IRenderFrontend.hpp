#pragma once
// job:
// hold the drawCommand Vector
// lookup table of lookup[EntityID] = drawCommand  vector id 
//
// scene manager flushes? -> updates the draw command vector 


// also holds array of Materials? 
 

// the drawCommand vector is the list of items that needs drawn in the scene 
// EntityID should lookup to HERE -> map to its draw command
//
// node spawns? 
// regester to mesh -> get mesh ID 
//  regester to texture for each texture, get handles 
// produce a draw command: 
// register that draw command HERE!
// stores WHAT to draw info (material, mesh, cull info, layer)
// -> MeshManager for meshHandle 
// -> itself for Material structs
//
// interpolaton server stores WHERE to draw info 
// both can be found with the EntityID 

// so its job is to link together a draw command:
// EntityID assigned by SceneManager perhaps? idk its inside the node 
// MeshHandle from MeshManager 
// MaterialHandle from textureManager 
// renderLayer from Node Type?
//
