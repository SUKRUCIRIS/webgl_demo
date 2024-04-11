#version 300 es
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec4 rgba;
out vec2 TexCoords;
out vec4 RGBA;
uniform mat4 camera;
void main()
{
  TexCoords = tex;
  RGBA = rgba;
  gl_Position = camera * vec4(pos, 1.0);
}