#include "text_render.h"
#include <algorithm>
// Şükrü Çiriş 2024

shader_program *ui_text_renderer::default_program = 0;

camera *ui_text_renderer::default_cam = 0;

ui_text_renderer::ui_text_renderer(const std::string &font_file, int height, int virtualsw, int virtualsh,
                                   int realsw, int realsh, GLint min_filter, GLint mag_filter)
{
  if (ui_text_renderer::default_program == 0)
  {
    default_program = new shader_program("./shaders/font.vs", "./shaders/font.fs", true);
  }
  if (ui_text_renderer::default_cam == 0)
  {
    vec3 pos = {0, 0, 0};
    default_cam = new camera_orthogonal(realsw, realsh, 0.0f, (float)virtualsw, 0.0f, (float)virtualsh, pos, -100.0f, 100.0f);
  }
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    emscripten_console_log("FT_Init_FreeType Error");
  }
  FT_Face face;
  if (FT_New_Face(ft, font_file.c_str(), 0, &face))
  {
    emscripten_console_log("FT_New_Face Error");
  }
  if (FT_Set_Pixel_Sizes(face, 0, height))
  {
    emscripten_console_log("FT_Set_Pixel_Sizes Error");
  }

  this->twidth = 0, this->theight = 0;
  for (unsigned char c = 32; c < 127; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      emscripten_console_log("FT_Load_Char Error");
    }

    this->sizex[c - 32] = face->glyph->bitmap.width;
    this->sizey[c - 32] = face->glyph->bitmap.rows;
    this->bearingx[c - 32] = face->glyph->bitmap_left;
    this->bearingy[c - 32] = face->glyph->bitmap_top;
    this->advancex[c - 32] = face->glyph->advance.x;
    this->twidth += face->glyph->bitmap.width;
    this->theight = std::max(this->theight, face->glyph->bitmap.rows);
  }

  this->twidth += 95; // extra 1 pixel between textures

  glGenTextures(1, &this->font_textures);
  glBindTexture(GL_TEXTURE_2D, this->font_textures);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, this->twidth, this->theight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  if (min_filter == GL_NEAREST_MIPMAP_NEAREST ||
      min_filter == GL_LINEAR_MIPMAP_NEAREST ||
      min_filter == GL_NEAREST_MIPMAP_LINEAR ||
      min_filter == GL_LINEAR_MIPMAP_LINEAR)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  int x = 0;

  for (unsigned char c = 32; c < 127; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      emscripten_console_log("FT_Load_Char Error");
    }
    if (face->glyph->bitmap.buffer != 0)
    {
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                      GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    }
    this->xoffset[c - 32] = (float)x / (float)this->twidth;
    x += face->glyph->bitmap.width + 1;
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
  this->virtualsw = virtualsw;
  this->virtualsh = virtualsh;
  this->realsw = realsw;
  this->realsh = realsh;
  this->VAO = 0;
  this->VBO = 0;
  this->EBO = 0;
  this->newdata = 0;
}
ui_text_renderer::~ui_text_renderer()
{
  glDeleteTextures(1, &(this->font_textures));
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteBuffers(1, &(this->EBO));
}
void ui_text_renderer::add_text(float startx, float starty, float z, float scale, vec4 rgba, const std::string &text)
{
  this->newdata = 1;
  GLuint *indices = (GLuint *)malloc(sizeof(GLuint) * 6 * text.size());
  unsigned int vertex_start = this->vertices.size() / 9;
  for (unsigned int i = 0; i < text.size(); i++)
  {
    indices[i * 6] = 2 + vertex_start + 4 * i;
    indices[i * 6 + 1] = 1 + vertex_start + 4 * i;
    indices[i * 6 + 2] = vertex_start + 4 * i;
    indices[i * 6 + 3] = vertex_start + 4 * i;
    indices[i * 6 + 4] = 3 + vertex_start + 4 * i;
    indices[i * 6 + 5] = 2 + vertex_start + 4 * i;
  }
  float old_startx = startx;
  for (unsigned int i = 0; i < text.size(); i++)
  {
    if (text[i] == '\n')
    {
      starty -= (this->theight + 3) * scale;
      startx = old_startx;
      continue;
    }
    char ascii = text[i];
    if (ascii < 32 || ascii > 126)
    {
      ascii = 32;
    }
    ascii = ascii - 32;
    float xpos = startx + this->bearingx[ascii] * scale;
    float ypos = starty - (this->sizey[ascii] - this->bearingy[ascii]) * scale;
    float w = this->sizex[ascii] * (float)scale;
    float h = this->sizey[ascii] * (float)scale;
    startx += (this->advancex[ascii] >> 6) * scale;
    GLfloat vertices[] = {
        xpos, ypos + h, z, this->xoffset[ascii], 0, rgba[0], rgba[1], rgba[2], rgba[3],                                            // 0
        xpos, ypos, z, this->xoffset[ascii], (float)this->sizey[ascii] / (float)this->theight, rgba[0], rgba[1], rgba[2], rgba[3], // 1
        xpos + w, ypos, z, this->xoffset[ascii] + (float)this->sizex[ascii] / (float)this->twidth,
        (float)this->sizey[ascii] / (float)this->theight, rgba[0], rgba[1], rgba[2], rgba[3],                                                 // 2
        xpos + w, ypos + h, z, this->xoffset[ascii] + (float)this->sizex[ascii] / (float)this->twidth, 0, rgba[0], rgba[1], rgba[2], rgba[3], // 3
    };
    this->vertices.insert(this->vertices.end(), vertices, vertices + 36);
  }
  this->indices.insert(this->indices.end(), indices, indices + 6 * text.size());
  free(indices);
}
void ui_text_renderer::get_text_size(int scale, float *width, float *height, const std::string &text)
{
  *width = 0;
  *height = 0;
  float startx = 0;
  float starty = 0;
  float old_startx = startx;
  for (unsigned int i = 0; i < text.size(); i++)
  {
    if (text[i] == '\n')
    {
      starty -= (this->theight + 3) * scale;
      startx = old_startx;
      continue;
    }
    char ascii = text[i];
    if (ascii < 32 || ascii > 126)
    {
      ascii = 32;
    }
    ascii = ascii - 32;
    float xpos = startx + this->bearingx[ascii] * scale;
    float ypos = starty - (this->sizey[ascii] - this->bearingy[ascii]) * scale;
    float w = this->sizex[ascii] * (float)scale;
    float h = this->sizey[ascii] * (float)scale;
    startx += (this->advancex[ascii] >> 6) * scale;
    *width = std::max(*width, xpos + w);
    *height = std::max(*height, ypos + h);
  }
}
void ui_text_renderer::render(const shader_program &program, camera &cam)
{
  if ((this->VAO == 0 || this->newdata == 1) && this->indices.size() > 0)
  {
    this->newdata = 0;
    glDeleteVertexArrays(1, &(this->VAO));
    glDeleteBuffers(1, &(this->VBO));
    glDeleteBuffers(1, &(this->EBO));
    glBindVertexArray(0);
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
    glGenBuffers(1, &(this->EBO));
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLfloat), this->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  if (this->VAO != 0)
  {
    program.use();
    cam.calculate();
    cam.use(program);
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D, this->font_textures);
    auto it = std::find(this->programs.begin(), this->programs.end(), program.get());
    if (it == this->programs.end())
    {
      this->programs.push_back(program.get());
      GLint uniform = glGetUniformLocation(program.get(), "font_textures");
      this->uniforms.push_back(uniform);
    }
    it = std::find(this->programs.begin(), this->programs.end(), program.get());
    int index = std::distance(this->programs.begin(), it);
    GLint *uniforms = this->uniforms.data();
    glUniform1i(uniforms[index], 31);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
void ui_text_renderer::clear()
{
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteBuffers(1, &(this->EBO));
  this->VAO = 0;
  this->VBO = 0;
  this->EBO = 0;
  this->vertices.clear();
  this->indices.clear();
}