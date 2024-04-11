#include "texture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "../../third_party/stb/stb_image.h"
// Şükrü Çiriş 2024

texture_manager::texture *texture_manager::create_texture(std::string path, GLenum texType, GLint min_filter, GLint mag_filter,
                                                          int index, int wraps, int wrapt)
{
  if (this->textures.size() >= 16)
  {
    return 0;
  }
  glBindTexture(texType, 0);
  texture *tex = new texture;
  tex->id = 0;
  tex->type = 0;
  int widthImg, heightImg, numColCh;
  stbi_set_flip_vertically_on_load(1);
  unsigned char *bytes = stbi_load(path.c_str(), &widthImg, &heightImg, &numColCh, 0);
  if (!bytes)
  {
    return 0;
  }
  GLenum format = GL_RGB;
  if (numColCh == 1)
  {
    format = GL_RED;
  }
  else if (numColCh == 3)
  {
    format = GL_RGB;
  }
  else if (numColCh == 4)
  {
    format = GL_RGBA;
  }
  glGenTextures(1, &(tex->id));
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(texType, tex->id);
  glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTexParameteri(texType, GL_TEXTURE_WRAP_S, wraps);
  glTexParameteri(texType, GL_TEXTURE_WRAP_T, wrapt);
  if (format == GL_RED)
  {
    glTexImage2D(texType, 0, GL_R8, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
  }
  else if (format == GL_RGB)
  {
    glTexImage2D(texType, 0, GL_RGB8, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
  }
  else if (format == GL_RGBA)
  {
    glTexImage2D(texType, 0, GL_RGBA8, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
  }
  if (min_filter == GL_NEAREST_MIPMAP_NEAREST ||
      min_filter == GL_LINEAR_MIPMAP_NEAREST ||
      min_filter == GL_NEAREST_MIPMAP_LINEAR ||
      min_filter == GL_LINEAR_MIPMAP_LINEAR)
  {
    glGenerateMipmap(texType);
  }
  stbi_image_free(bytes);
  glBindTexture(texType, 0);
  tex->type = texType;
  this->textures.push_back(tex);
  this->indices[this->textures.size() - 1] = index;
  return tex;
}
void texture_manager::delete_texture(texture *id)
{
  auto it = std::find(this->textures.begin(), this->textures.end(), id);
  int index = std::distance(this->textures.begin(), it);
  for (int i = index; i < 31; i++)
  {
    this->indices[i] = this->indices[i + 1];
  }
  this->textures.erase(this->textures.begin() + index);
  glDeleteTextures(1, &(id->id));
  delete id;
}
void texture_manager::use(const shader_program &program)
{
  program.use();
  for (unsigned int i = 0; i < this->textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + this->indices[i]);
    glBindTexture(this->textures[i]->type, this->textures[i]->id);
  }
  auto it = std::find(this->programs.begin(), this->programs.end(), program.get());
  if (it == this->programs.end())
  {
    this->programs.push_back(program.get());
    GLint uniform = glGetUniformLocation(program.get(), "textures");
    this->uniforms.push_back(uniform);
  }
  it = std::find(this->programs.begin(), this->programs.end(), program.get());
  int index = std::distance(this->programs.begin(), it);
  glUniform1iv(this->uniforms[index], 16, this->indices);
}
texture_manager::~texture_manager()
{
  for (int i = 0; i < this->textures.size(); i++)
  {
    delete this->textures[i];
  }
}