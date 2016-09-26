#version 430


layout (location = 0 ) out vec4 colorOut;


smooth in vec3 cameraDir;
smooth in vec3 lightDir;
smooth in vec4 pos;

layout (std140, binding = 4) uniform Data
{
	mat4 MVP; 				//	64
	float heightScale;		//   4
	int invertHMap;			//   4
	vec3 ViewPosition;		//  16
	vec3 LightPosition;
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
};

uniform sampler2D ground;
void main()
{
	vec3 normalDirection = vec3 (1,0,0);
	vec2 texC;
	if(pos.z < -0.01 && pos.z > -102.18){
		texC = pos.zy;
		if(pos.x>1) normalDirection = vec3(1,0,0);
		else normalDirection = vec3(-1,0,0);
	}else{
		texC = pos.xy;
		if(pos.z>-0.01) normalDirection = vec3(0,0,1);
		else normalDirection = vec3(0,0,-1);
	}
	vec3 textureColor = texture2D(ground, texC/10.0,0).xyz;
	//vec3 cameraDir = normalize(cameraDir);
	//vec3 lightDir = normalize (lightDir);
	
	float diffuseTerm = dot(lightDir, normalDirection);
	vec3 difLight = vec3(1,1,1) * diffuseTerm; 
	
	colorOut = vec4((difLight+1.0)*textureColor,1);
}