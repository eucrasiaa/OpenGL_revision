#pragma once

// Prevent GL headers from polluting macros
#define GLAPIENTRY
#define APIENTRY
#define WINAPI

// Block common conflicting extensions
#define GL_GLEXT_LEGACY

#define __glad_h_ 1
#define __GLAD_GL_H_

#include <GL/gl.h>
#include <GL/glext.h>

#include <glad/glad.h>
