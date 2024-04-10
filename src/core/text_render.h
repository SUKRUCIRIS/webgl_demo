#pragma once
#include "tp.h"
#include "shader.h"
#include <vector>
#include <string>
// Şükrü Çiriş 2024
class text_renderer
{
private:
  text_renderer() = delete;
  GLuint font_textures;
  GLuint VAO, VBO, EBO;
  std::vector<GLfloat> vertices; // 3 vertex coord, 2 texture coord, 3 rgb
  std::vector<GLuint> indices;
  int sizex[95];
  int sizey[95];
  int bearingx[95];
  int bearingy[95];
  int advancex[95];
  float xoffset[95];
  mat4 projection;
  std::vector<GLuint> programs; // i will save uniforms here. i wont find their locations everytime i render for performance
  std::vector<GLint> uniforms;
  unsigned char newdata;
  int screenwidth, screenheight;
  int realsw, realsh;
  unsigned int twidth, theight;

public:
  text_renderer(const std::string &font_file, int height, int screenwidth, int screenheight,
                int realsw, int realsh, GLint min_filter, GLint mag_filter);
  ~text_renderer();
  void add_text(float startx, float starty, float z, float scale, vec4 rgba, const std::string &text);
  void get_text_size(int scale, float *width, float *height, const std::string &text);
  void render(shader_program *program = 0);
  void clear();
};