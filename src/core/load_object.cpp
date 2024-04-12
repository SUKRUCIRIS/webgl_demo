#include "load_object.h"
// Şükrü Çiriş 2024
loaded_model::loaded_model(std::string path, bool flip_order, object_manager &obj_manager, texture_manager &text_manager, float texture_start_index)
{
  emscripten_console_log("Loading 3D model...");
  this->obj_manager = &obj_manager;
  this->text_manager = &text_manager;
  aiScene *scene = 0;
  if (!flip_order)
  {
    scene = (aiScene *)aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality |
                                                      aiProcess_TransformUVCoords | aiProcess_PreTransformVertices | aiProcess_GenBoundingBoxes);
  }
  else
  {
    scene = (aiScene *)aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality |
                                                      aiProcess_TransformUVCoords | aiProcess_PreTransformVertices | aiProcess_GenBoundingBoxes |
                                                      aiProcess_FlipWindingOrder);
  }
  this->box.mMax.x = -1000000;
  this->box.mMax.y = -1000000;
  this->box.mMax.z = -1000000;
  this->box.mMin.x = 1000000;
  this->box.mMin.y = 1000000;
  this->box.mMin.z = 1000000;
  for (unsigned int i = 0; i < scene->mNumMaterials; i++)
  {
    aiString texturePath = {};
    aiColor4D diffuseColor = {};
    if (aiGetMaterialTexture(scene->mMaterials[i], aiTextureType_DIFFUSE, 0, &texturePath, 0, 0, 0, 0, 0, 0) == aiReturn_SUCCESS)
    {
      emscripten_console_log("Model has diffuse texture...");
      unsigned char embedded = 0;
      for (unsigned int k = 0; k < scene->mNumTextures; k++)
      {
        if (strcmp(scene->mTextures[k]->mFilename.data, texturePath.data) == 0)
        {
          embedded = 1;
          int width, height;
          unsigned char *data;
          width = scene->mTextures[k]->mWidth;
          height = scene->mTextures[k]->mHeight;
          if (height == 0)
          {
            break;
          }
          data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 4);
          for (int j = 0; j < width * height; j++)
          {
            data[j * 4] = scene->mTextures[k]->pcData[j].r;
            data[j * 4 + 1] = scene->mTextures[k]->pcData[j].g;
            data[j * 4 + 2] = scene->mTextures[k]->pcData[j].b;
            data[j * 4 + 3] = scene->mTextures[k]->pcData[j].a;
          }
          this->textures[i] = text_manager.create_texture_memory(data, width, height, GL_TEXTURE_2D, GL_NEAREST,
                                                                 GL_NEAREST, (int)texture_start_index + (int)i, GL_REPEAT, GL_REPEAT);
          free(data);
          break;
        }
      }
      if (embedded == 0)
      {
        emscripten_console_log("Model has external diffuse texture...");
        std::string ext_path = "./models/" + std::string(texturePath.data);
        std::replace(ext_path.begin(), ext_path.end(), '\\', '/');
        emscripten_console_log(ext_path.c_str());
        this->textures[i] = text_manager.create_texture(ext_path, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST,
                                                        (int)texture_start_index + (int)i, GL_REPEAT, GL_REPEAT);
      }
    }
    else if (aiGetMaterialTexture(scene->mMaterials[i], aiTextureType_BASE_COLOR, 0, &texturePath, 0, 0, 0, 0, 0, 0) == aiReturn_SUCCESS)
    {
      emscripten_console_log("Model has base color texture...");
      unsigned char embedded = 0;
      for (unsigned int k = 0; k < scene->mNumTextures; k++)
      {
        if (strcmp(scene->mTextures[k]->mFilename.data, texturePath.data) == 0)
        {
          embedded = 1;
          int width, height;
          unsigned char *data;
          width = scene->mTextures[k]->mWidth;
          height = scene->mTextures[k]->mHeight;
          if (height == 0)
          {
            break;
          }
          data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 4);
          for (int j = 0; j < width * height; j++)
          {
            data[j * 4] = scene->mTextures[k]->pcData[j].r;
            data[j * 4 + 1] = scene->mTextures[k]->pcData[j].g;
            data[j * 4 + 2] = scene->mTextures[k]->pcData[j].b;
            data[j * 4 + 3] = scene->mTextures[k]->pcData[j].a;
          }
          this->textures[i] = text_manager.create_texture_memory(data, width, height, GL_TEXTURE_2D, GL_NEAREST,
                                                                 GL_NEAREST, (int)texture_start_index + (int)i, GL_REPEAT, GL_REPEAT);
          free(data);
          break;
        }
      }
      if (embedded == 0)
      {
        emscripten_console_log("Model has external base color texture...");
        std::string ext_path = "./models/" + std::string(texturePath.data);
        std::replace(ext_path.begin(), ext_path.end(), '\\', '/');
        emscripten_console_log(ext_path.c_str());
        this->textures[i] = text_manager.create_texture(ext_path, GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST,
                                                        (int)texture_start_index + (int)i, GL_REPEAT, GL_REPEAT);
      }
    }
    else if (aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS)
    {
      emscripten_console_log("Model has material...");
      unsigned char data[4];
      data[0] = (unsigned char)std::max(0, std::min(255, (int)floorf(diffuseColor.r * 256.0f)));
      data[1] = (unsigned char)std::max(0, std::min(255, (int)floorf(diffuseColor.g * 256.0f)));
      data[2] = (unsigned char)std::max(0, std::min(255, (int)floorf(diffuseColor.b * 256.0f)));
      data[3] = (unsigned char)std::max(0, std::min(255, (int)floorf(diffuseColor.a * 256.0f)));
      this->textures.push_back(text_manager.create_texture_memory(data, 1, 1, GL_TEXTURE_2D, GL_NEAREST,
                                                                  GL_NEAREST, (int)texture_start_index + (int)i, GL_REPEAT, GL_REPEAT));
    }
  }
  for (unsigned int i = 0; i < scene->mNumMeshes; i++)
  {
    unsigned int vertex_number = scene->mMeshes[i]->mNumVertices;
    GLfloat *vertices = (GLfloat *)malloc(sizeof(GLfloat) * vertex_number * 9);
    for (unsigned int k = 0; k < vertex_number; k++)
    {
      vertices[k * 9] = scene->mMeshes[i]->mVertices[k].x;
      vertices[k * 9 + 1] = scene->mMeshes[i]->mVertices[k].y;
      vertices[k * 9 + 2] = scene->mMeshes[i]->mVertices[k].z;

      vertices[k * 9 + 3] = scene->mMeshes[i]->mTextureCoords[0][k].x;
      vertices[k * 9 + 4] = scene->mMeshes[i]->mTextureCoords[0][k].y;

      vertices[k * 9 + 5] = scene->mMeshes[i]->mNormals[k].x;
      vertices[k * 9 + 6] = scene->mMeshes[i]->mNormals[k].y;
      vertices[k * 9 + 7] = scene->mMeshes[i]->mNormals[k].z;

      vertices[k * 9 + 8] = scene->mMeshes[i]->mMaterialIndex + texture_start_index;
    }
    unsigned int indice_number = 0;
    for (unsigned int k = 0; k < scene->mMeshes[i]->mNumFaces; k++)
    {
      indice_number += scene->mMeshes[i]->mFaces[k].mNumIndices;
    }
    GLuint *indices = (GLuint *)malloc(sizeof(GLuint) * indice_number);
    unsigned int index = 0;
    for (unsigned int k = 0; k < scene->mMeshes[i]->mNumFaces; k++)
    {
      for (unsigned int j = 0; j < scene->mMeshes[i]->mFaces[k].mNumIndices; j++)
      {
        indices[index] = scene->mMeshes[i]->mFaces[k].mIndices[j];
        index++;
      }
    }
    this->meshes.push_back(obj_manager.create_object(vertices, vertex_number, indices, indice_number,
                                                     scene->mMeshes[i]->mMaterialIndex + texture_start_index));
    free(vertices);
    free(indices);
    if (this->box.mMax.x < scene->mMeshes[i]->mAABB.mMax.x)
    {
      this->box.mMax.x = scene->mMeshes[i]->mAABB.mMax.x;
    }
    if (this->box.mMax.y < scene->mMeshes[i]->mAABB.mMax.y)
    {
      this->box.mMax.y = scene->mMeshes[i]->mAABB.mMax.y;
    }
    if (this->box.mMax.z < scene->mMeshes[i]->mAABB.mMax.z)
    {
      this->box.mMax.z = scene->mMeshes[i]->mAABB.mMax.z;
    }
    if (this->box.mMin.x > scene->mMeshes[i]->mAABB.mMin.x)
    {
      this->box.mMin.x = scene->mMeshes[i]->mAABB.mMin.x;
    }
    if (this->box.mMin.y > scene->mMeshes[i]->mAABB.mMin.y)
    {
      this->box.mMin.y = scene->mMeshes[i]->mAABB.mMin.y;
    }
    if (this->box.mMin.z > scene->mMeshes[i]->mAABB.mMin.z)
    {
      this->box.mMin.z = scene->mMeshes[i]->mAABB.mMin.z;
    }
  }
  aiReleaseImport(scene);
}
void loaded_model::scale(vec3 v)
{
  for (int i = 0; i < this->meshes.size(); i++)
  {
    this->obj_manager->scale_object(this->meshes[i], v);
  }
}
void loaded_model::rotate(float angle, vec3 axis)
{
  for (int i = 0; i < this->meshes.size(); i++)
  {
    this->obj_manager->rotate_object(this->meshes[i], angle, axis);
  }
}
void loaded_model::translate(vec3 v)
{
  for (int i = 0; i < this->meshes.size(); i++)
  {
    this->obj_manager->translate_object(this->meshes[i], v);
  }
}