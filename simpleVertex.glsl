/* 
SimpleVertex.glsl

Vertex shader with position, color, normal and ModelViewProject
input and color output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

uniform mat4 ModelViewProjection;  // = projection * view * model
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;
uniform mat4 ModelView;
uniform bool IsTexture;

out vec4 color;
out vec3 fPosition;
out vec3 fNormal;
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
  gl_Position = ModelViewProjection * vPosition;
  fPosition =  (ModelView * vPosition).xyz;
fNormal = normalize(NormalMatrix *vNormal);
  }
  
 