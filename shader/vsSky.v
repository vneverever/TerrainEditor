#version 430 core
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

uniform mat4 Vie;
uniform mat4 Pro;

layout (location = 0) in vec4 aPosition;

smooth out vec3 eyeDirection;

void main() {
    mat4 inverseProjection = inverse(ProjectionMatrix);
    mat3 inverseModelview = transpose(mat3(ViewMatrix));
    vec3 unprojected = (inverseProjection * aPosition).xyz;
    eyeDirection = inverseModelview * unprojected;

    gl_Position = aPosition;
}