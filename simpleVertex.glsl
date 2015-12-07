/* 
SimpleVertex.glsl

Vertex shader with position, color, normal and ModelViewProject
input and color output.

Mike Barnes
8/17/2013
*/

# version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;  // not used
layout (location = 3)in vec2 vTexCoord;

uniform mat4 ModelViewProjection;  // = projection * view * model
uniform mat3 NormalMatrix;
uniform bool IsTexture;

out vec4 color;
out vec3 vs_worldpos;
out vec3 vs_normal;
out vec2 vs_texCoord;

void main() {
  if(IsTexture)
  {
  vs_texCoord = vTexCoord;
  color = vec4(0,0,0,0);
  }
  else
  {
  vs_texCoord = vec2(0,0);
  color = vColor;
  }
  vec4 position =ModelViewProjection * vPosition;
  gl_Position = position;
  vs_worldpos=position.xyz;
 vs_normal = NormalMatrix * vNormal;
  }
  
 