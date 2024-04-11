#include "app.h"
// Şükrü Çiriş 2024
loopdata *loda;

void prepareloop(loopdata *ld)
{
  loda = ld;
  vec4 color = {0, 0, 0, 1};
  loda->trenderer->add_text(0, 15, 1, 0.5f, color, "3D Temperature Display Demo");
  loda->texmanager->create_texture("./textures/box.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void mainloop(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, loda->scwidth, loda->scheight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  loda->texmanager->use(*object_manager::get_default_program());
  loda->objmanager->render();
  loda->trenderer->render();
  object_manager::get_default_camera()->run_input_free_camera3D(loda->window);
  glfwPollEvents();
  glfwSwapBuffers(loda->window);
}