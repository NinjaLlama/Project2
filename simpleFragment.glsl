/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;
in vec2 vs_texCoord;

uniform vec3 HeadLightPosition;
uniform vec3 HeadLightIntensity;
uniform vec3 PointLightPosition;
uniform vec3 PointLightIntensity;

uniform bool HeadLightOn; // toggles set in application
uniform bool PointLightOn;
uniform bool AmbientLightOn;
uniform bool DebugOn; 
vec3 ambientColor  = vec3(1.0, 0.0, 0.0); //red ambient 
vec3 diffuseColor  = vec3(0.0, 1.0, 0.0); // green diffuse

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

out vec4 fragColor;

vec3 vLight(vec3 LightPosition, vec3 LightIntensity, bool directional) {
    float ambient = 0.2f;
    // scale directional ambient
	float diffuse = 0.0f;
    // compute diffuse in all cases
	vec3 n, s;
    // normal, light source
	if (directional)
	{ 
		s = normalize(LightPosition); 
	}
    else {
    // point light has no ambient
		s = normalize(LightPosition - fPosition);
		ambient = 0.0f;

	}


	

	n = normalize(fNormal);
    diffuse = max(dot(s, n), 0.0);

	if(DebugOn && AmbientLightOn)
		return ambient * ambientColor + diffuse * diffuseColor;
	else
	{
		if (DebugOn)
		{
			ambient=0.0f;
			return ambient * ambientColor + diffuse * diffuseColor; 
		}
		else if(AmbientLightOn)
		{
			return ambient * LightIntensity + diffuse * LightIntensity;
		}
		else
		{
			// reflected light
			ambient=0.0f;
			return ambient * LightIntensity + diffuse * LightIntensity;
		}

	}


	 
}

void main() {


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
  {
	vec3 tempColor = vec3(color) * 0.5f;
	//initial value
	if (HeadLightOn){
		tempColor += vLight(HeadLightPosition,HeadLightIntensity, true);
	}
	if (PointLightOn) 
	{
		tempColor += vLight(PointLightPosition, PointLightIntensity, false);
	}
	
	fragColor = vec4(tempColor, 1.0);
  }
  



  }