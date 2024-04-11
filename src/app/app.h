#pragma once
#include "../core/core.h"
// Şükrü Çiriş 2024
typedef struct loopdata
{
  GLFWwindow *window = 0;
  ui_text_renderer *trenderer = 0;
  int scwidth = 0, scheight = 0;
  object_manager *objmanager = 0;
  texture_manager *texmanager = 0;
} loopdata;

void mainloop(void);

void prepareloop(loopdata *ld);