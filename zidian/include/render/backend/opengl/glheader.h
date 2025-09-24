#pragma once

#ifdef _WIN32
#include "render/backend/opengl/glad/glad.h"
#elif _WIN64
#include "render/backend/opengl/glad/glad.h"
#elif __CYGWIN__
#include "render/backend/opengl/glad/glad.h"
#elif __ANDROID__
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#elif __linux__
#include "render/backend/opengl/glad/glad.h"
#elif _UNIX
#include "render/backend/opengl/glad/glad.h"
#elif __APPLE__
#include "render/backend/opengl/glad/glad.h"
#endif


// #ifndef _GLAD_
// #define _GLAD_
// #include "glad/glad.h"
// #endif





