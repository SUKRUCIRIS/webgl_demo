#version 300 es
precision mediump float;
out vec4 FragColor;
in vec3 texCoords;
uniform samplerCube cubemap;

void main()
{
  FragColor=texture(cubemap, texCoords);
}