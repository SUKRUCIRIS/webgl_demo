#include "app.h"
#include <emscripten.h>

loopdata *loda;

void prepareloop(loopdata *ld)
{
  loda = ld;
}

void mainloop(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glfwPollEvents();
  poll_events(loda->window);
  glfwSwapBuffers(loda->window);
}