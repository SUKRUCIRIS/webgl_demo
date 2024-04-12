#include "camera.h"
// Şükrü Çiriş 2024
camera_perspective::camera_perspective(int screenwidth, int screenheight, vec3 position, float FOVdeg, float nearPlane, float farPlane, float speed,
                                       float sensitivity, float angle, vec3 angle_axis)
{
  this->orientation[0] = 0;
  this->orientation[1] = 0;
  this->orientation[2] = -1;
  this->up[0] = 0;
  this->up[1] = 1;
  this->up[2] = 0;
  this->speed = speed;
  this->sensitivity = sensitivity;
  this->width = screenwidth;
  this->height = screenheight;
  this->position[0] = position[0];
  this->position[1] = position[1];
  this->position[2] = position[2];
  this->FOVdeg = FOVdeg;
  this->nearPlane = nearPlane;
  this->farPlane = farPlane;
  this->basespeed = speed;
  if (angle != 0)
  {
    glm_vec3_rotate(this->orientation, glm_rad(angle), angle_axis);
  }
  glm_mat4_identity(this->view);
  glm_mat4_identity(this->projection);
  glm_mat4_identity(this->result);
}
void camera::run_input_rotate_camera3D(GLFWwindow *window)
{
  double mouseX;
  double mouseY;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && this->lastmousex != -1 && this->lastmousey != -1)
  {
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float rotX = this->sensitivity * (float)(mouseY - this->lastmousey) / this->height;
    float rotY = this->sensitivity * (float)(mouseX - this->lastmousex) / this->width;
    vec3 oldor;
    oldor[0] = this->orientation[0];
    oldor[1] = this->orientation[1];
    oldor[2] = this->orientation[2];
    vec3 cross;
    glm_vec3_cross(this->orientation, this->up, cross);
    glm_vec3_normalize(cross);
    glm_vec3_rotate(this->orientation, glm_rad(-rotX), cross);
    if (fabs(glm_vec3_angle(this->orientation, this->up) - glm_rad(90.0f)) > glm_rad(85.0f))
    {
      this->orientation[0] = oldor[0];
      this->orientation[1] = oldor[1];
      this->orientation[2] = oldor[2];
    }
    glm_vec3_rotate(this->orientation, glm_rad(-rotY), this->up);
  }
  glfwGetCursorPos(window, &mouseX, &mouseY);
  this->lastmousex = mouseX;
  this->lastmousey = mouseY;
}
void camera::run_input_free_camera3D(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    vec3 direction;
    glm_vec3_scale(this->orientation, this->speed, direction);
    glm_vec3_add(this->position, direction, this->position);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    vec3 direction;
    vec3 cross;
    glm_vec3_cross(this->orientation, this->up, cross);
    glm_normalize_to(cross, cross);
    glm_vec3_scale(cross, -(this->speed), direction);
    glm_vec3_add(this->position, direction, this->position);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    vec3 direction;
    glm_vec3_scale(this->orientation, -(this->speed), direction);
    glm_vec3_add(this->position, direction, this->position);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    vec3 direction;
    vec3 cross;
    glm_vec3_cross(this->orientation, this->up, cross);
    glm_normalize_to(cross, cross);
    glm_vec3_scale(cross, this->speed, direction);
    glm_vec3_add(this->position, direction, this->position);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    vec3 direction;
    glm_vec3_scale(this->up, this->speed, direction);
    glm_vec3_add(this->position, direction, this->position);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    vec3 direction;
    glm_vec3_scale(this->up, -this->speed, direction);
    glm_vec3_add(this->position, direction, this->position);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    this->speed = this->basespeed * 2;
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
    this->speed = this->basespeed;
  }
  this->run_input_rotate_camera3D(window);
}
void camera::calculate()
{
  vec3 sum;
  glm_vec3_add(this->position, this->orientation, sum);
  glm_lookat(this->position, sum, this->up, this->view);
  this->calculate_projection();
  glm_mat4_mul(this->projection, this->view, this->result);
}
void camera::use(const shader_program &program)
{
  auto it = std::find(this->programs.begin(), this->programs.end(), program.get());
  if (it == this->programs.end())
  {
    this->programs.push_back(program.get());
    GLint uniform = glGetUniformLocation(program.get(), "camera");
    this->uniforms.push_back(uniform);
    uniform = glGetUniformLocation(program.get(), "camPos");
    this->uniforms.push_back(uniform);
    uniform = glGetUniformLocation(program.get(), "view");
    this->uniforms.push_back(uniform);
    uniform = glGetUniformLocation(program.get(), "projection");
    this->uniforms.push_back(uniform);
  }
  it = std::find(this->programs.begin(), this->programs.end(), program.get());
  int index = std::distance(this->programs.begin(), it);
  glUniformMatrix4fv(this->uniforms[index * 4], 1, GL_FALSE, this->result[0]);
  glUniform3f(this->uniforms[index * 4 + 1], this->position[0], this->position[1], this->position[2]);
  glUniformMatrix4fv(this->uniforms[index * 4 + 2], 1, GL_FALSE, this->view[0]);
  glUniformMatrix4fv(this->uniforms[index * 4 + 3], 1, GL_FALSE, this->projection[0]);
}
void camera_perspective::calculate_projection()
{
  glm_perspective(glm_rad(this->FOVdeg), (float)this->width / this->height, this->nearPlane, this->farPlane, this->projection);
}
void camera_orthogonal::calculate_projection()
{
  glm_ortho(this->left, this->right, this->bottom, this->top, this->nearPlane, this->farPlane, this->projection);
}
camera_orthogonal::camera_orthogonal(int screenwidth, int screenheight, float left, float right, float bottom, float top, vec3 position,
                                     float nearPlane, float farPlane, float speed, float sensitivity, float angle, vec3 angle_axis)
{
  this->orientation[0] = 0;
  this->orientation[1] = 0;
  this->orientation[2] = -1;
  this->up[0] = 0;
  this->up[1] = 1;
  this->up[2] = 0;
  this->speed = speed;
  this->sensitivity = sensitivity;
  this->width = screenwidth;
  this->height = screenheight;
  this->position[0] = position[0];
  this->position[1] = position[1];
  this->position[2] = position[2];
  this->left = left;
  this->right = right;
  this->bottom = bottom;
  this->top = top;
  this->nearPlane = nearPlane;
  this->farPlane = farPlane;
  this->basespeed = speed;
  if (angle != 0)
  {
    glm_vec3_rotate(this->orientation, glm_rad(angle), angle_axis);
  }
  glm_mat4_identity(this->view);
  glm_mat4_identity(this->projection);
  glm_mat4_identity(this->result);
}
void camera::set_position(vec3 pos)
{
  glm_vec3_copy(pos, this->position);
}
float *camera::get_position()
{
  return this->position;
}