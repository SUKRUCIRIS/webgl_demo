#pragma once
#include "tp.h"
#include <vector>
#include <string>
#include "shader.h"
// Şükrü Çiriş 2024

class texture_manager
{
private:
  struct texture
  {
    GLuint id;
    GLenum type;
  };
  std::vector<texture *> textures;
  int indices[16] = {0};
  std::vector<GLuint> programs; // i will save uniforms here. i wont find their locations everytime i render for performance
  std::vector<GLint> uniforms;

public:
  ~texture_manager();
  texture *create_texture(std::string path, GLenum texType, GLint min_filter, GLint mag_filter, int index, int wraps, int wrapt);
  void delete_texture(texture *id);
  void use(const shader_program &program);
};