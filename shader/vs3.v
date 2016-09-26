#version 430
	
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec2 hMapC;	
	
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

smooth out vec4 pos;
smooth out vec3 normalDirection;
smooth out vec2 texC;
smooth out vec3 worldNormal;
smooth out vec4 eyeVec;

uniform sampler2D hMap;
uniform float heightScale;
uniform int invertHMap;

float height( vec2 tPos )
{
    float result = vec4(texelFetch(hMap, ivec2(tPos),0)).y*2-1;
	if(invertHMap==1) result = 1-result;
	result = result*heightScale;
    return result;
}
 
void main () {
	pos = vec4(position,1.0);
	texC = texCoord;
	
    gl_Position = MVP * vec4 (position, 1.0);

	vec3 off = vec3(1.0, 1.0, 0.0);
	float hL = height(hMapC.xy - off.xz);
	float hR = height(hMapC.xy + off.xz);
	float hD = height(hMapC.xy - off.zy);
	float hU = height(hMapC.xy + off.zy);

	vec3 testNormal;
	// deduce terrain normal
	testNormal.x = hL - hR;
	testNormal.z = hD - hU;
	testNormal.y = 2.0;
	testNormal = normalize(testNormal);
	worldNormal = testNormal;
	
	normalDirection = NormalMatrix * testNormal;
	eyeVec = ModelViewMatrix * -(ModelViewMatrix * pos);
}