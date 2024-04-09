#include "./core/core.h"
#include "./app/app.h"
#include <emscripten.h>
#include <emscripten/html5.h>

int main()
{
  emscripten_set_window_title("Şükrü Çiriş Engine");
  // you have to create context in main for emscripten (It didn't work otherwise)
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  int width = 0;
  int height = 0;
  int fs = 0;
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
  glClearColor(1.0f, 0.24f, 0.31f, 1.0f);

  loopdata ld;
  ld.window = window;

  prepareloop(&ld);

  emscripten_set_main_loop(mainloop, 0, 1);

  delete_window(window);
  return 0;
}