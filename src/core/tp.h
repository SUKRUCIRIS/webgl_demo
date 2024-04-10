#pragma once
// Şükrü Çiriş 2024
#include "../../third_party/opengl/gl32.h"
#include "../../third_party/glfw/glfw3.h"
#include "../../third_party/stb/stb_image.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "../../third_party/cglm/include/cglm/cglm.h"
#include "../../third_party/assimp/include/assimp/cimport.h"
#include "../../third_party/assimp/include/assimp/postprocess.h"
#include "../../third_party/assimp/include/assimp/scene.h"
#include "../../third_party/freetype/ft2build.h"
#include FT_FREETYPE_H
#include <emscripten.h>
#include <emscripten/html5.h>
