#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>


// the info every shader gets
struct SceneUniforms {
    // std140 layout
    glm::mat4 viewMatrix;       
    glm::mat4 projectionMatrix; 
    glm::mat4 viewProjMatrix;   
    glm::mat4 invViewProjMatrix;
    glm::vec4 cameraPosition;   // (xyz = pos, w = near plane)
    glm::vec4 cameraDirection;  //(xyz = forward)
    glm::vec4 screenParams;     // (xy = resolution, zw = 1/resolution)
    glm::vec4 timeParams;       //(x = time, y = deltaTime, z = sin(time))
    glm::vec4 renderPassData;   //(x = pass index, y = mip bias)
};


struct RenderPassUBO {
    // Created ONCE at engine init
    GLuint ubo;
    
    void Initialize() {
        glCreateBuffers(1, &ubo);
        glNamedBufferStorage(ubo, sizeof(SceneUniforms), nullptr,
                           GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
    }
    
    // once per frame
    void Update(const SceneUniforms& data) {
        void* ptr = glMapNamedBufferRange(ubo, 0, sizeof(SceneUniforms),
                                         GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT |
                                         GL_MAP_INVALIDATE_BUFFER_BIT);
        memcpy(ptr, &data, sizeof(SceneUniforms));
        glUnmapNamedBuffer(ubo);
    }
};

/*
layout(std140, binding = 1) uniform SceneData {
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_ViewProj;
    mat4 u_InvViewProj;
    vec4 u_CameraPos;
    vec4 u_CameraDir;
    vec4 u_ScreenParams;
    vec4 u_Time;
    vec4 u_PassData;
};
*/
