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

uniform mat4 ModelViewProjection;  // = projection * view * model
uniform mat3 NormalMatrix;

out vec4 color;
out vec3 vs_worldpos;
out vec3 vs_normal;

void main() {
  color = vColor;
  vec4 position =ModelViewProjection * vPosition;
  gl_Position = position;
  vs_worldpos=position.xyz;
 vs_normal = NormalMatrix * vNormal;
  }
  
 