#pragma once
#include "../core/core.h"

typedef struct loopdata
{
  GLFWwindow *window;
} loopdata;

void mainloop(void);

void prepareloop(loopdata *ld);