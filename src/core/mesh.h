#pragma once
#include "tp.h"
#include <vector>
#include "shader.h"
#include "camera.h"
// Şükrü Çiriş 2024

class object_manager
{
private:
  struct object
  {
    size_t vertex_number;
    size_t indice_number;
    size_t vertex_start;
    size_t indice_start;
    mat4 model;
    mat4 normal;
  };
  std::vector<object *> objects;
  GLuint VAO, VBO, EBO;
  std::vector<GLfloat> vertices; // 3 vertex coord, 2 texture coord, 3 normal coord, 1 texture id
  std::vector<GLuint> indices;
  bool subdata;
  mat4 translation;
  mat4 rotation;
  mat4 scaling;
  mat4 model;
  mat4 normal;
  std::vector<GLuint> programs; // i will save uniforms here. i wont find their locations everytime i render for performance
  std::vector<GLint> uniforms;
  size_t object_number;
  size_t indice_number;
  const static std::string default_vertex_shader;
  const static std::string default_fragment_shader;
  static shader_program *default_program;
  static camera *default_cam;
  void object_apply_model_matrix(object *obj);

public:
  object_manager(int realsw, int realsh);
  ~object_manager();
  void scale(vec3 v);
  void rotate(float angle, vec3 axis);
  void translate(vec3 v);
  void set_position(vec3 v);
  float *get_position();
  size_t create_object(GLfloat *vertices, size_t vertex_number, GLuint *indices, size_t indice_number, GLfloat texture_index);
  void scale_object(size_t index, vec3 v);
  void rotate_object(size_t index, float angle, vec3 axis);
  void translate_object(size_t index, vec3 v);
  void delete_object(size_t index);
  void prepare_render();
  void render(const shader_program &program = *default_program, camera &cam = *default_cam);
  static camera *get_default_camera();
  size_t create_cube_object(float texture_index = 0, vec3 pos = 0, vec3 scaling = 0);
};