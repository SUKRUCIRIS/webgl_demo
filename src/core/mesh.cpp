#include "mesh.h"
// Şükrü Çiriş 2024

shader_program *object_manager::default_program = 0;
camera *object_manager::default_cam = 0;

object_manager::object_manager(int realsw, int realsh)
{
  if (object_manager::default_program == 0)
  {
    object_manager::default_program = new shader_program("./shaders/object.vs", "./shaders/object.fs", true);
  }
  if (object_manager::default_cam == 0)
  {
    vec3 pos = {0, 5, 20};
    object_manager::default_cam = new camera_perspective(realsw, realsh, pos, 60, 0.1f, 1000, 0.1f, 100);
  }
  this->VAO = 0;
  this->VBO = 0;
  this->EBO = 0;
  this->subdata = false;
  glm_mat4_identity(this->model);
  glm_mat4_identity(this->normal);
  glm_mat4_identity(this->translation);
  glm_mat4_identity(this->rotation);
  glm_mat4_identity(this->scaling);
  this->object_number = 0;
  this->indice_number = 0;
}
object_manager::~object_manager()
{
  for (int i = 0; i < object_number; i++)
  {
    delete this->objects[i];
  }
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteBuffers(1, &(this->EBO));
}
void object_manager::scale(vec3 v)
{
  mat4 tmp;
  glm_scale_make(tmp, v);
  glm_mat4_mul(tmp, this->scaling, this->scaling);
}
void object_manager::rotate(float angle, vec3 axis)
{
  mat4 tmp;
  glm_rotate_make(tmp, glm_rad(angle), axis);
  glm_mat4_mul(tmp, this->rotation, this->rotation);
}
void object_manager::translate(vec3 v)
{
  glm_vec3_add(v, this->translation[3], this->translation[3]);
}
void object_manager::set_position(vec3 v)
{
  glm_translate_make(this->translation, v);
}
float *object_manager::get_position()
{
  return this->translation[3];
}
object_manager::object *object_manager::create_object(GLfloat *vertices, size_t vertex_number, GLuint *indices, size_t indice_number, GLfloat texture_index)
{
  if (vertices == 0 || vertex_number == 0)
  {
    return 0;
  }
  GLuint *noindice = 0;
  if (indices == 0 || indice_number == 0)
  {
    indice_number = vertex_number;
    noindice = (GLuint *)malloc(sizeof(GLuint) * indice_number);
    for (unsigned int i = 0; i < indice_number; i++)
    {
      noindice[i] = i;
    }
    indices = noindice;
  }
  object *x = new object;
  glm_mat4_identity(x->model);
  glm_mat4_identity(x->normal);
  x->vertex_number = vertex_number;
  x->indice_number = indice_number;
  GLfloat oldtext_index = vertices[8];
  for (unsigned int i = 0; i < vertex_number; i++)
  {
    vertices[9 * i + 8] = texture_index;
  }
  x->vertex_start = this->vertices.size() / 9;
  x->indice_start = this->indices.size();
  this->vertices.insert(this->vertices.end(), vertices, vertices + vertex_number * 9);
  for (unsigned int i = 0; i < indice_number; i++)
  {
    indices[i] += x->vertex_start;
  }
  this->indices.insert(this->indices.end(), indices, indices + indice_number);
  for (unsigned int i = 0; i < indice_number; i++)
  {
    indices[i] -= x->vertex_start;
  }
  for (unsigned int i = 0; i < vertex_number; i++)
  {
    vertices[9 * i + 8] = oldtext_index;
  }
  free(noindice);
  this->objects.push_back(x);
  this->indice_number = this->indices.size();
  this->object_number = this->objects.size();
  return x;
}
void object_manager::scale_object(object *id, vec3 v)
{
  glm_scale(id->model, v);
  object_apply_model_matrix(id);
}
void object_manager::rotate_object(object *id, float angle, vec3 axis)
{
  glm_rotate(id->model, glm_rad(angle), axis);
  object_apply_model_matrix(id);
}
void object_manager::translate_object(object *id, vec3 v)
{
  glm_translate(id->model, v);
  object_apply_model_matrix(id);
}
void object_manager::delete_object(object *id)
{
  auto it = std::find(this->objects.begin(), this->objects.end(), id);
  if (it == this->objects.end())
  {
    return;
  }
  size_t index = std::distance(this->objects.begin(), it);
  object *obj = this->objects[index];
  this->vertices.erase(this->vertices.begin() + obj->vertex_start * 9, this->vertices.begin() + obj->vertex_start * 9 + obj->vertex_number * 9);
  this->indices.erase(this->indices.begin() + obj->indice_start, this->indices.begin() + obj->indice_start + obj->indice_number);
  GLuint *indices = this->indices.data();
  if (index != this->objects.size() - 1)
  {
    for (unsigned int i = index + 1; i < this->objects.size(); i++)
    {
      this->objects[i]->vertex_start -= obj->vertex_number;
      this->objects[i]->indice_start -= obj->indice_number;

      for (unsigned int i2 = this->objects[i]->indice_start; i2 < this->objects[i]->indice_start + this->objects[i]->indice_number; i2++)
      {
        indices[i2] -= obj->vertex_number;
      }
    }
  }
  this->objects.erase(this->objects.begin() + index);
  delete obj;
}
void object_manager::render(const shader_program &program, camera &cam)
{
  if (this->object_number > 0)
  {
    program.use();
    cam.calculate();
    cam.use(program);
    auto it = std::find(this->programs.begin(), this->programs.end(), program.get());
    if (it == this->programs.end())
    {
      this->programs.push_back(program.get());
      GLint uniform = glGetUniformLocation(program.get(), "model");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "normalMatrix");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "tempdivisor1");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "tempdivisor2");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "part1temp");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "part2temp");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "part3temp");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "degree20color");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "degree60color");
      this->uniforms.push_back(uniform);
      uniform = glGetUniformLocation(program.get(), "maketemp");
      this->uniforms.push_back(uniform);
    }
    it = std::find(this->programs.begin(), this->programs.end(), program.get());
    int index = std::distance(this->programs.begin(), it);
    mat4 *tmp[] = {&this->translation, &this->rotation, &this->scaling};
    glm_mat4_mulN(tmp, 3, this->model);
    glm_mat4_inv(this->model, this->normal);
    glm_mat4_transpose(this->normal);
    glUniformMatrix4fv(this->uniforms[index * 10], 1, GL_FALSE, this->model[0]);
    glUniformMatrix4fv(this->uniforms[index * 10 + 1], 1, GL_FALSE, this->normal[0]);
    glUniform1f(uniforms[index * 10 + 2], this->tempdivisor1);
    glUniform1f(uniforms[index * 10 + 3], this->tempdivisor2);
    glUniform1f(uniforms[index * 10 + 4], this->part1temp);
    glUniform1f(uniforms[index * 10 + 5], this->part2temp);
    glUniform1f(uniforms[index * 10 + 6], this->part3temp);
    glUniform3f(uniforms[index * 10 + 7], this->degree20color[0], this->degree20color[1], this->degree20color[2]);
    glUniform3f(uniforms[index * 10 + 8], this->degree60color[0], this->degree60color[1], this->degree60color[2]);
    glUniform1f(uniforms[index * 10 + 9], this->maketemp);

    if (this->subdata)
    {
      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(GLfloat), this->vertices.data());
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      this->subdata = false;
    }

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indice_number, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
void object_manager::object_apply_model_matrix(object *obj)
{
  glm_mat4_inv(obj->model, obj->normal);
  glm_mat4_transpose(obj->normal);
  vec4 tmp = {0, 0, 0, 0};
  GLfloat *vertices = this->vertices.data();
  for (unsigned int i = 0; i < obj->vertex_number; i++)
  {
    vec4 vert = {vertices[(obj->vertex_start + i) * 9], vertices[(obj->vertex_start + i) * 9 + 1], vertices[(obj->vertex_start + i) * 9 + 2], 1};
    glm_mat4_mulv(obj->model, vert, tmp);
    glm_vec3_copy(tmp, &(vertices[(obj->vertex_start + i) * 9]));

    vec4 norm = {vertices[(obj->vertex_start + i) * 9 + 5], vertices[(obj->vertex_start + i) * 9 + 6], vertices[(obj->vertex_start + i) * 9 + 7], 1};
    glm_mat4_mulv(obj->normal, norm, tmp);
    glm_vec3_copy(tmp, &(vertices[(obj->vertex_start + i) * 9 + 5]));
  }
  glm_mat4_identity(obj->model);
  glm_mat4_identity(obj->normal);
  this->subdata = true;
}
void object_manager::prepare_render()
{
  glDeleteVertexArrays(1, &(this->VAO));
  glDeleteBuffers(1, &(this->VBO));
  glDeleteBuffers(1, &(this->EBO));
  glBindVertexArray(0);
  if (this->objects.size() > 0)
  {
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
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  this->subdata = false;
}
camera *object_manager::get_default_camera()
{
  return object_manager::default_cam;
}
shader_program *object_manager::get_default_program()
{
  return object_manager::default_program;
}
object_manager::object *object_manager::create_cube_object(float texture_index, vec3 pos, vec3 scaling)
{
  static GLfloat cube_vertices[] = {
      -0.5f, -0.5f, -0.5f, 0, 0, 0, 0, -1, 0, // A 0
      -0.5f, 0.5f, -0.5f, 0, 1, 0, 0, -1, 0,  // B 1
      0.5f, 0.5f, -0.5f, 1, 1, 0, 0, -1, 0,   // C 2
      0.5f, -0.5f, -0.5f, 1, 0, 0, 0, -1, 0,  // D 3
      -0.5f, -0.5f, 0.5f, 0, 0, 0, 0, 1, 0,   // E 4
      0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 1, 0,    // F 5
      0.5f, 0.5f, 0.5f, 1, 1, 0, 0, 1, 0,     // G 6
      -0.5f, 0.5f, 0.5f, 0, 1, 0, 0, 1, 0,    // H 7

      -0.5f, 0.5f, -0.5f, 0, 1, -1, 0, 0, 0,  // D 8
      -0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 0, 0, // A 9
      -0.5f, -0.5f, 0.5f, 1, 0, -1, 0, 0, 0,  // E 10
      -0.5f, 0.5f, 0.5f, 1, 1, -1, 0, 0, 0,   // H 11
      0.5f, -0.5f, -0.5f, 0, 0, 1, 0, 0, 0,   // B 12
      0.5f, 0.5f, -0.5f, 0, 1, 1, 0, 0, 0,    // C 13
      0.5f, 0.5f, 0.5f, 1, 1, 1, 0, 0, 0,     // G 14
      0.5f, -0.5f, 0.5f, 1, 0, 1, 0, 0, 0,    // F 15

      -0.5f, -0.5f, -0.5f, 0, 0, 0, -1, 0, 0, // A 16
      0.5f, -0.5f, -0.5f, 1, 0, 0, -1, 0, 0,  // B 17
      0.5f, -0.5f, 0.5f, 1, 1, 0, -1, 0, 0,   // F 18
      -0.5f, -0.5f, 0.5f, 0, 1, 0, -1, 0, 0,  // E 19
      0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0, 0,    // C 20
      -0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 0, 0,   // D 21
      -0.5f, 0.5f, 0.5f, 1, 1, 0, 1, 0, 0,    // H 22
      0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0, 0,     // G 23
  };
  // index data
  static GLuint cube_indices[] = {
      // front and back
      2, 1, 0,
      0, 3, 2,
      6, 5, 4,
      4, 7, 6,
      // left and right
      9, 8, 11,
      11, 10, 9,
      14, 13, 12,
      12, 15, 14,
      // bottom and top
      18, 17, 16,
      16, 19, 18,
      22, 21, 20,
      20, 23, 22};

  object *id = this->create_object(cube_vertices, 24, cube_indices, 36, texture_index);
  if (pos != 0)
  {
    this->translate_object(id, pos);
  }
  if (scaling != 0)
  {
    this->scale_object(id, scaling);
  }

  return id;
}
void object_manager::set_temp_settings(float tempdiv1, float tempdiv2, vec3 degree20color, vec3 degree60color)
{
  this->tempdivisor1 = tempdiv1;
  this->tempdivisor2 = tempdiv2;
  if (degree20color != 0)
  {
    glm_vec3_copy(degree20color, this->degree20color);
  }
  if (degree60color != 0)
  {
    glm_vec3_copy(degree60color, this->degree60color);
  }
}
void object_manager::set_temp(float part1, float part2, float part3)
{
  this->part1temp = part1;
  this->part2temp = part2;
  this->part3temp = part3;
}
void object_manager::enable_temp()
{
  this->maketemp = 1;
}
void object_manager::disable_temp()
{
  this->maketemp = 0;
}
float object_manager::get_temp()
{
  return this->maketemp;
}