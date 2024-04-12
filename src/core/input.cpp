#include "input.h"
// Şükrü Çiriş 2024

bool input_manager::get_key_pressed(int key)
{
  return pressed[key];
}
bool input_manager::get_key_released(int key)
{
  return released[key];
}
bool input_manager::get_key_down(int key)
{
  return down[key];
}
bool input_manager::get_key_up(int key)
{
  return up[key];
}
void input_manager::poll_events(GLFWwindow *window)
{
  for (int key = 0; key < GLFW_KEY_LAST + 1; key++)
  {
    if (glfwGetKey(window, key) == GLFW_PRESS)
    {
      pressed[key] = 0;
      if (down[key] == 0)
      {
        pressed[key] = 1;
      }
      released[key] = 0;
      up[key] = 0;
      down[key] = 1;
    }
    else
    {
      released[key] = 0;
      if (down[key] == 1)
      {
        released[key] = 1;
      }
      pressed[key] = 0;
      up[key] = 1;
      down[key] = 0;
    }
  }
}