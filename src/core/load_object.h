#pragma once
#include "tp.h"
#include "mesh.h"
#include "texture.h"
#include <vector>
#include <string>
// Şükrü Çiriş 2024
class loaded_model
{
private:
  loaded_model() = delete;

public:
  std::vector<object_manager::object *> meshes;
  std::vector<texture_manager::texture *> textures;
  aiAABB box;
  object_manager *obj_manager;
  texture_manager *text_manager;
  loaded_model(std::string path, bool flip_order, object_manager &obj_manager, texture_manager &text_manager, float texture_start_index);
  void scale(vec3 v);
  void rotate(float angle, vec3 axis);
  void translate(vec3 v);
};