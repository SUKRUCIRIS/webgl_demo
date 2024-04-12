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
  shader_program(std::string vertex_source, std::string fragment_source, bool file);
  ~shader_program();
  void use() const;
  GLuint get() const;
};