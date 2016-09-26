#version 430 core
layout (location = 0) in vec3 position;

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

uniform vec4 start;
smooth out vec4 pos;


void main()
{
    gl_Position = MVP * vec4(position, 1.0);
	//pos = vec4(position, 1.0);

}