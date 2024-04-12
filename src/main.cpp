#include "./core/core.h"
#include "./app/app.h"
// Şükrü Çiriş 2024
int main()
{
  emscripten_set_window_title("Şükrü Çiriş Engine");
  // you have to create context in main for emscripten according to my experiences
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  int width = 0;
  int height = 0;
  emscripten_get_canvas_element_size("canvas", &width, &height);
  GLFWwindow *window = glfwCreateWindow(width, height, "Şükrü Çiriş Engine", 0, 0);
  glfwSwapInterval(1);
  glfwMakeContextCurrent(window);
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepthf(1.0f);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CCW);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0f, 0.24f, 0.31f, 1.0f);

  loopdata ld;
  ld.window = window;
  ld.scheight = height;
  ld.scwidth = width;
  ld.trenderer = new ui_text_renderer("./fonts/arial.ttf", 128, 1920, 1080, width, height, GL_LINEAR, GL_LINEAR);
  ld.objmanager = new object_manager(width, height);
  ld.texmanager = new texture_manager();
  ld.sb = new skybox("./textures/skybox/px.png", "./textures/skybox/nx.png",
                     "./textures/skybox/py.png", "./textures/skybox/ny.png",
                     "./textures/skybox/pz.png", "./textures/skybox/nz.png",
                     object_manager::get_default_camera());
  ld.inpmanager = new input_manager();

  prepareloop(&ld);

  emscripten_set_main_loop(mainloop, 0, 1);

  delete ld.inpmanager;
  delete ld.sb;
  delete ld.texmanager;
  delete ld.objmanager;
  delete ld.trenderer;
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}