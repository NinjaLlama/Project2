/* 
SimpleVertex.glsl

Vertex shader with position, color, normal and ModelViewProject
input and color output.

Mike Barnes
8/17/2013
*/
in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

uniform mat4 ModelViewProjection;  // = projection * view * model
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix;
uniform mat4 ModelView;

out vec4 color;
out vec3 fPosition;
out vec3 fNormal;

void main() {
  color = vColor;
  gl_Position = ModelViewProjection * vPosition;
  fPosition =  (ModelView * vPosition).xyz;
fNormal =   vNormal;

  }
 