#pragma once
#include "tp.h"
#include <vector>
#include "shader.h"
// Şükrü Çiriş 2024

class camera
{
protected:
  camera(){};
  mat4 result;
  vec3 position;
  vec3 orientation;
  vec3 up;
  float speed;
  float basespeed;
  float sensitivity;
  float nearPlane;
  float farPlane;
  float lastmousex = -1;
  float lastmousey = -1;
  int width;
  int height;
  mat4 view;
  mat4 projection;
  std::vector<GLuint> programs; // i will save uniforms here. i wont find their locations everytime i render for performance
  std::vector<GLint> uniforms;
  virtual void calculate_projection() = 0;

public:
  void run_input_rotate_camera3D(GLFWwindow *window);
  void run_input_free_camera3D(GLFWwindow *window);
  void calculate();
  void use(const shader_program &program);
  void set_position(vec3 pos);
  float *get_position(); // vec3
};

class camera_perspective : public camera
{
private:
  camera_perspective() = delete;
  float FOVdeg;

protected:
  virtual void calculate_projection();

public:
  camera_perspective(int screenwidth, int screenheight, vec3 position, float FOVdeg, float nearPlane, float farPlane, float speed = 0.1f,
                     float sensitivity = 100, float angle = 0, vec3 angle_axis = 0);
};

class camera_orthogonal : public camera
{
private:
  camera_orthogonal() = delete;
  float left, right, bottom, top;

protected:
  virtual void calculate_projection();

public:
  camera_orthogonal(int screenwidth, int screenheight, float left, float right, float bottom, float top, vec3 position,
                    float nearPlane, float farPlane, float speed = 1, float sensitivity = 100, float angle = 0, vec3 angle_axis = 0);
};