#include "skybox.h"
#include "../../third_party/stb/stb_image.h"
// Şükrü Çiriş 2024

static const float skyboxVertices[] =
    {
        //   Coordinates
        -1.0f, -1.0f, 1.0f,  //        7--------6
        1.0f, -1.0f, 1.0f,   //       /|       /|
        1.0f, -1.0f, -1.0f,  //      4--------5 |
        -1.0f, -1.0f, -1.0f, //      | |      | |
        -1.0f, 1.0f, 1.0f,   //      | 3------|-2
        1.0f, 1.0f, 1.0f,    //      |/       |/
        1.0f, 1.0f, -1.0f,   //      0--------1
        -1.0f, 1.0f, -1.0f};

static const unsigned int skyboxIndices[] =
    {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3};

shader_program *skybox::default_program = 0;

skybox::skybox(const char *right_texture, const char *left_texture, const char *top_texture,
               const char *bottom_texture, const char *front_texture, const char *back_texture,
               camera *cam)
{
  if (skybox::default_program == 0)
  {
    default_program = new shader_program("./shaders/skybox.vs", "./shaders/skybox.fs", true);
  }
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->EBO);
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  this->cam = cam;
  glGenTextures(1, &this->cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // These are very important to prevent seams
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  int width, height, nrChannels;
  {
    unsigned char *data = stbi_load(right_texture, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(false);
    GLenum format = GL_RGB;
    if (nrChannels == 1)
    {
      format = GL_RED;
    }
    else if (nrChannels == 3)
    {
      format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
      format = GL_RGBA;
    }
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        0,
        GL_RGBA,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data);
    stbi_image_free(data);
  }
  {
    unsigned char *data = stbi_load(left_texture, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(false);
    GLenum format = GL_RGB;
    if (nrChannels == 1)
    {
      format = GL_RED;
    }
    else if (nrChannels == 3)
    {
      format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
      format = GL_RGBA;
    }
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1,
        0,
        GL_RGBA,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data);
    stbi_image_free(data);
  }
  {
    unsigned char *data = stbi_load(top_texture, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(false);
    GLenum format = GL_RGB;
    if (nrChannels == 1)
    {
      format = GL_RED;
    }
    else if (nrChannels == 3)
    {
      format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
      format = GL_RGBA;
    }
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2,
        0,
        GL_RGBA,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data);
    stbi_image_free(data);
  }
  {
    unsigned char *data = stbi_load(bottom_texture, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(false);
    GLenum format = GL_RGB;
    if (nrChannels == 1)
    {
      format = GL_RED;
    }
    else if (nrChannels == 3)
    {
      format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
      format = GL_RGBA;
    }
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3,
        0,
        GL_RGBA,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data);
    stbi_image_free(data);
  }
  {
    unsigned char *data = stbi_load(front_texture, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(false);
    GLenum format = GL_RGB;
    if (nrChannels == 1)
    {
      format = GL_RED;
    }
    else if (nrChannels == 3)
    {
      format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
      format = GL_RGBA;
    }
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4,
        0,
        GL_RGBA,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data);
    stbi_image_free(data);
  }
  {
    unsigned char *data = stbi_load(back_texture, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(false);
    GLenum format = GL_RGB;
    if (nrChannels == 1)
    {
      format = GL_RED;
    }
    else if (nrChannels == 3)
    {
      format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
      format = GL_RGBA;
    }
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5,
        0,
        GL_RGBA,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data);
    stbi_image_free(data);
  }
}
skybox::~skybox()
{
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteBuffers(1, &(this->EBO));
  glDeleteTextures(1, &(this->cubemap));
}
void skybox::render(const shader_program &program)
{
  program.use();
  mat3 x;
  this->cam->calculate();
  glm_mat4_pick3(this->cam->view, x);
  glm_mat4_identity(this->result);
  glm_mat4_ins3(x, this->result);
  glm_mat4_mul(this->cam->projection, this->result, this->result);
  auto it = std::find(this->programs.begin(), this->programs.end(), program.get());
  if (it == this->programs.end())
  {
    this->programs.push_back(program.get());
    GLint uniform = glGetUniformLocation(program.get(), "cam");
    this->uniforms.push_back(uniform);
    uniform = glGetUniformLocation(program.get(), "cubemap");
    this->uniforms.push_back(uniform);
  }
  it = std::find(this->programs.begin(), this->programs.end(), program.get());
  int index = std::distance(this->programs.begin(), it);
  glUniformMatrix4fv(this->uniforms[index * 2], 1, GL_FALSE, this->result[0]);
  glUniform1i(this->uniforms[index * 2 + 1], 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap);
  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}