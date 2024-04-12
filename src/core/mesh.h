#pragma once
#include "tp.h"
#include <vector>
#include "shader.h"
#include "camera.h"
// Şükrü Çiriş 2024
class object_manager
{
public:
  struct object
  {
    size_t vertex_number;
    size_t indice_number;
    size_t vertex_start;
    size_t indice_start;
    mat4 model;
    mat4 normal;
  };

private:
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
  static shader_program *default_program;
  static camera *default_cam;
  void object_apply_model_matrix(object *obj);
  float tempdivisor1 = 0, tempdivisor2 = 1, part1temp = 20, part2temp = 20, part3temp = 20;
  vec3 degree20color = {0, 0, 1};
  vec3 degree60color = {1, 0, 0};
  float maketemp = 0;

public:
  object_manager(int realsw, int realsh);
  ~object_manager();
  void scale(vec3 v);
  void rotate(float angle, vec3 axis);
  void translate(vec3 v);
  void set_position(vec3 v);
  float *get_position();
  object *create_object(GLfloat *vertices, size_t vertex_number, GLuint *indices, size_t indice_number, GLfloat texture_index);
  void scale_object(object *id, vec3 v);
  void rotate_object(object *id, float angle, vec3 axis);
  void translate_object(object *id, vec3 v);
  void delete_object(object *id);
  void prepare_render();
  void render(const shader_program &program = *default_program, camera &cam = *default_cam);
  static camera *get_default_camera();
  static shader_program *get_default_program();
  object *create_cube_object(float texture_index = 0, vec3 pos = 0, vec3 scaling = 0);
  void set_temp_settings(float tempdiv1, float tempdiv2, vec3 degree20color = 0, vec3 degree60color = 0);
  void set_temp(float part1, float part2, float part3);
  void enable_temp();
  void disable_temp();
  float get_temp();
};