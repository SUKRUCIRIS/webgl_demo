#pragma once
#include "shader.h"
#include "camera.h"
#include "tp.h"
#include <vector>
#include <string>
// Şükrü Çiriş 2024
class ui_text_renderer
{
private:
  ui_text_renderer() = delete;
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
  std::vector<GLuint> programs; // i will save uniforms here. i wont find their locations everytime i render for performance
  std::vector<GLint> uniforms;
  unsigned char newdata;
  int virtualsw, virtualsh;
  int realsw, realsh;
  unsigned int twidth, theight;
  static shader_program *default_program;
  static camera *default_cam;

public:
  ui_text_renderer(const std::string &font_file, int height, int virtualsw, int virtualsh,
                   int realsw, int realsh, GLint min_filter, GLint mag_filter);
  ~ui_text_renderer();
  void add_text(float startx, float starty, float z, float scale, vec4 rgba, const std::string &text);
  void get_text_size(float scale, float *width, float *height, const std::string &text);
  void render(const shader_program &program = *default_program, camera &cam = *default_cam);
  void clear();
};