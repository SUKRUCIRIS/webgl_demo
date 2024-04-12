#include "app.h"
// Şükrü Çiriş 2024
loopdata *loda;

void prepareloop(loopdata *ld)
{
  loda = ld;

  vec4 color = {1, 0.55f, 0, 1};
  float width = 0, height = 0;
  loda->trenderer->get_text_size(0.5f, &width, &height, "3D Temperature Display Web Demo");
  loda->trenderer->add_text(0, 1080 - height, 1, 0.5f, color, "3D Temperature Display Web Demo");

  loda->trenderer->get_text_size(0.25f, &width, &height, "Part 1: 22 Degree\nPart 2: 45 Degree\nPart 3: 58 Degree");
  loda->trenderer->add_text(0, 980 - height, 1, 0.25f, color, "Part 1: 22 Degree\nPart 2: 45 Degree\nPart 3: 58 Degree");

  loda->trenderer->get_text_size(0.25f, &width, &height, "Press T to enable/disable temperature view");
  loda->trenderer->add_text(0, 800 - height, 1, 0.25f, color, "Press T to enable/disable temperature view");

  loaded_model machine("./models/ac/ac.gltf", true, *loda->objmanager, *loda->texmanager, 0);
  float acheight = machine.box.mMax.y - machine.box.mMin.y;
  float acwidth = machine.box.mMax.x - machine.box.mMin.x;
  float targetheight = 10;
  vec3 acscaling = {targetheight / acheight, targetheight / acheight, targetheight / acheight};
  acwidth *= targetheight / acheight;
  machine.scale(acscaling);
  loda->objmanager->set_temp(22, 45, 58);
  loda->objmanager->set_temp_settings(-acwidth / 6, acwidth / 6);
  loda->objmanager->prepare_render();
  loda->objmanager->disable_temp();
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
  loda->inpmanager->poll_events(loda->window);
  if (loda->inpmanager->get_key_pressed(GLFW_KEY_T))
  {
    if (loda->objmanager->get_temp() == 1)
    {
      loda->objmanager->disable_temp();
    }
    else
    {
      loda->objmanager->enable_temp();
    }
  }
  glfwSwapBuffers(loda->window);
}