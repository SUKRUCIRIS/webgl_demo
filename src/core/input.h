#pragma once
#include "tp.h"
// Şükrü Çiriş 2024
class input_manager
{
private:
  bool pressed[GLFW_KEY_LAST + 1] = {false};
  bool released[GLFW_KEY_LAST + 1] = {false};
  bool up[GLFW_KEY_LAST + 1] = {false};
  bool down[GLFW_KEY_LAST + 1] = {false};

public:
  bool get_key_pressed(int key);
  bool get_key_released(int key);
  bool get_key_down(int key);
  bool get_key_up(int key);
  void poll_events(GLFWwindow *window);
};