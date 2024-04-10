#pragma once
#include "tp.h"
#include <string>
// Şükrü Çiriş 2024

class shader_program
{
private:
  shader_program() = delete;
  GLuint program = 0;

public:
  shader_program(const std::string &vertex_source, const std::string &fragment_source);
  ~shader_program();
  void use();
  GLuint get();
};