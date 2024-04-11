#version 300 es
precision mediump float;
in vec2 TexCoords;
in vec4 RGBA;
out vec4 FragColor;
uniform sampler2D font_textures;
void main(){
  FragColor = vec4(RGBA.rgb,texture(font_textures, TexCoords).r*RGBA.a);
}