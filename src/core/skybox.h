#pragma once
#include "tp.h"
#include "camera.h"
// Şükrü Çiriş 2024
class skybox
{
private:
  skybox() = delete;
  GLuint VAO, VBO, EBO, cubemap;
  camera *cam;
  mat4 result;
  std::vector<GLuint> programs; // i will save uniforms here. i wont find their locations everytime i render for performance
  std::vector<GLint> uniforms;
  static shader_program *default_program;

public:
  skybox(const char *right_texture, const char *left_texture, const char *top_texture,
         const char *bottom_texture, const char *front_texture, const char *back_texture,
         camera *cam);
  ~skybox();
  void render(const shader_program &program = *default_program);
};