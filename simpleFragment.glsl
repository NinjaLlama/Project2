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
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform sampler2D Texture5;
uniform sampler2D Texture6;
uniform int id;
uniform bool Tex1;
uniform bool Tex2;
uniform bool Tex3;
uniform bool Tex4;
uniform bool Tex5;
uniform bool IsTexture;

void main() {

float ambient=10.0f;//scale the ambient light
vec3 light_direction=normalize(light_position - vs_worldpos);
vec3 normal=normalize(vs_normal);
float diffuse=max(0.0,dot(normal,light_direction));
if(IsTexture)
{
	if(Tex1)
	fragColor = texture(Texture1, vs_texCoord); 
	else if(Tex2)
	fragColor = texture(Texture2, vs_texCoord);
	else if(Tex3)
	fragColor = texture(Texture4, vs_texCoord);
	else if(Tex4)
	fragColor = texture(Texture3, vs_texCoord);
	else if(Tex5)
	fragColor = texture(Texture5, vs_texCoord);
	else
	fragColor = texture(Texture6, vs_texCoord);
	
}
else
  fragColor = ambient * (color_ambient + diffuse) *color;
  
  



  }