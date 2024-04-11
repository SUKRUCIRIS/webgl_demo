#include "shader.h"
#include <fstream>
#include <streambuf>
// Şükrü Çiriş 2024

shader_program::shader_program(std::string vertex_source, std::string fragment_source, bool file)
{
  if (file)
  {
    std::ifstream t(vertex_source);
    vertex_source = std::string((std::istreambuf_iterator<char>(t)),
                                std::istreambuf_iterator<char>());

    t = std::ifstream(fragment_source);
    fragment_source = std::string((std::istreambuf_iterator<char>(t)),
                                  std::istreambuf_iterator<char>());
  }
  GLint gl_shader_result = GL_FALSE;
  int InfoLogLength;

  emscripten_console_log("Compiling vertex shader...");
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vss = vertex_source.c_str();
  glShaderSource(vertexShader, 1, &vss, 0);
  glCompileShader(vertexShader);

  // check for vertex shader errors
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &gl_shader_result);
  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::string msg(InfoLogLength + 1, '\0');
    glGetShaderInfoLog(vertexShader, InfoLogLength, NULL, (GLchar *)msg.c_str());
    emscripten_console_log(("Vertex shader error: " + msg).c_str());
  }

  emscripten_console_log("Compiling fragment shader...");
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fss = fragment_source.c_str();
  glShaderSource(fragmentShader, 1, &fss, 0);
  glCompileShader(fragmentShader);

  // check for fragment shader errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &gl_shader_result);
  glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::string msg(InfoLogLength + 1, '\0');
    glGetShaderInfoLog(fragmentShader, InfoLogLength, NULL, (GLchar *)msg.c_str());
    emscripten_console_log(("Fragment shader error: " + msg).c_str());
  }

  emscripten_console_log("Linking program...");
  this->program = glCreateProgram();
  glAttachShader(this->program, vertexShader);
  glAttachShader(this->program, fragmentShader);
  glLinkProgram(this->program);

  // Check the program
  glGetProgramiv(this->program, GL_LINK_STATUS, &gl_shader_result);
  glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 1)
  {
    std::string msg(InfoLogLength + 1, '\0');
    glGetProgramInfoLog(this->program, InfoLogLength, NULL, (GLchar *)msg.c_str());
    emscripten_console_log(("Shader program error: " + msg).c_str());
  }

  glDetachShader(this->program, vertexShader);
  glDetachShader(this->program, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}
shader_program::~shader_program()
{
  glDeleteProgram(this->program);
}
void shader_program::use() const
{
  glUseProgram(this->program);
}
GLuint shader_program::get() const
{
  return this->program;
}