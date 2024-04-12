#include "app.h"
// Şükrü Çiriş 2024
loopdata *loda;

void prepareloop(loopdata *ld)
{
  loda = ld;

  vec4 color = {0, 1, 0, 1};
  float width = 0, height = 0;
  loda->trenderer->get_text_size(0.5f, &width, &height, "3D Temperature Display Demo");
  loda->trenderer->add_text(0, 1080 - height, 1, 0.5f, color, "3D Temperature Display Demo");

  loaded_model machine("./models/ac/ac.gltf", true, *loda->objmanager, *loda->texmanager, 0);
  float acheight = machine.box.mMax.y - machine.box.mMin.y;
  float targetheight = 10;
  vec3 acscaling = {targetheight / acheight, targetheight / acheight, targetheight / acheight};
  machine.scale(acscaling);
  loda->objmanager->prepare_render();
}

void mainloop(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, loda->scwidth, loda->scheight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  loda->texmanager->use(*object_manager::get_default_program());
  loda->objmanager->render();
  loda->sb->render();
  loda->trenderer->render();
  object_manager::get_default_camera()->run_input_free_camera3D(loda->window);
  glfwPollEvents();
  glfwSwapBuffers(loda->window);
}