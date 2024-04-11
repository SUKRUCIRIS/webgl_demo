#include "app.h"
// Şükrü Çiriş 2024
loopdata *loda;

void prepareloop(loopdata *ld)
{
  loda = ld;
  vec4 color = {0, 0, 0, 1};
  loda->trenderer->add_text(0, 15, 1, 0.5f, color, "3D Temperature Display");
}

void mainloop(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, loda->scwidth, loda->scheight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  loda->objmanager->render();
  loda->trenderer->render();
  glfwPollEvents();
  glfwSwapBuffers(loda->window);
}