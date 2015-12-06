/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec4 color;
out vec4 fragColor;
in vec3 vs_worldpos;
in vec3 vs_normal;
in vec2 vs_texCoord;

//define light properties
uniform vec4 color_ambient=vec4(0.1,0.1,0.1,1.0);
uniform vec4 color_diffuse=vec4(0.7,0.7,0.7,0.1);
uniform vec3 light_position=vec3(0.0f,5000.0f,0.0f);
uniform sampler2D Texture;
uniform bool IsTexture;

void main() {

float ambient=10.0f;//scale the ambient light
vec3 light_direction=normalize(light_position - vs_worldpos);
vec3 normal=normalize(vs_normal);
float diffuse=max(0.0,dot(normal,light_direction));
if(IsTexture)
	fragColor = texture(Texture, vs_texCoord); 
else
  fragColor = ambient * (color_ambient + diffuse) *color;
  
  



  }