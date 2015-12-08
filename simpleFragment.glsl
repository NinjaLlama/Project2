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


uniform vec3 HeadLightPosition;
uniform vec3 HeadLightIntensity;
uniform vec3 PointLightPosition;
uniform vec3 PointLightIntensity;

uniform bool HeadLightOn; // toggles set in application
uniform bool PointLightOn;
uniform bool DebugOn; 
vec3 ambientColor  = vec3(1.0, 0.0, 0.0); //red ambient 
vec3 diffuseColor  = vec3(0.0, 1.0, 0.0); // green diffuse
out vec4 fragColor;

vec3 vLight(vec3 LightPosition, vec3 LightIntensity, bool directional) {
    float ambient = 0.5f;
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

	if (DebugOn)
	 return ambient * ambientColor + diffuse * diffuseColor; 
	 else 
	 return ambient * LightIntensity + diffuse * LightIntensity;

    // reflected light
	return ambient * LightIntensity + diffuse * LightIntensity;




	 
}



void main() {
	vec3 tempColor = vec3(color) * 0.1f;
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



