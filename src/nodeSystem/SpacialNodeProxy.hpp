#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
struct Vec3Proxy {
        glm::vec3& vec;
        bool& dirty;

        // proxy sub-component for .x, .y, .z
        struct Component {
            glm::vec3& v; bool& d; int idx;
            void operator=(float val) { v[idx] = val; d = true; }
            operator float() const { return v[idx]; }
        };

        Component x() { return {vec, dirty, 0}; }
        Component y() { return {vec, dirty, 1}; }
        Component z() { return {vec, dirty, 2}; }

        // Whole vector assignment: node.position = glm::vec3(1,2,3);
        void operator=(const glm::vec3& other) { vec = other; dirty = true; }
        operator glm::vec3() const { return vec; }
    };

struct RotationProxy {
        glm::quat& quat;
        bool& dirty;

        // Intercepts edits to individual Euler axes (X, Y, Z degrees or radians)
        struct EulerComponent {
            glm::quat& q; bool& d; int idx;
            
            void operator=(float val) {
              //quat to eular
              glm::vec3 euler = glm::degrees(glm::eulerAngles(q));
              euler[idx] = val;
              //writeback
              q = glm::quat(glm::radians(euler));
              d = true; 
            }
            operator float() const { return glm::degrees(glm::eulerAngles(q))[idx]; }
        };

        EulerComponent x() { return {quat, dirty, 0}; }
        EulerComponent y() { return {quat, dirty, 1}; }
        EulerComponent z() { return {quat, dirty, 2}; }

        // Allows replacing the whole quaternion natively
        void operator=(const glm::quat& other) { quat = other; dirty = true; }
        operator glm::quat() const { return quat; }
    };
