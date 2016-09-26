#version 430
layout (location = 0) in vec3 posWater;

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
uniform float waterLevel;

smooth out vec3 cameraDir;
smooth out vec3 lightDir;
uniform float timer;

smooth out vec4 pos;

void main () {
	pos = vec4(posWater.x,waterLevel+sin(timer/6), posWater.z,1.0);
    gl_Position = MVP * pos;

	cameraDir = normalize(ViewPosition-pos.xyz);
	lightDir = normalize(LightPosition.xyz - pos.xyz);
}