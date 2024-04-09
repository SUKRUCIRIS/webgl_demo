#include "./core/core.h"

int main()
{
  printf("started\n");
  GLFWwindow *window = create_window(0, 0, 1, 1, 0);
  while (!glfwWindowShouldClose((GLFWwindow *)window))
  {
    glfwPollEvents();
    poll_events((GLFWwindow *)window);
  }
  delete_window(window);
  return 0;
}