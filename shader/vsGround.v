#version 430
layout (location = 0) in vec2 posGround;

smooth out vec3 cameraDir;
smooth out vec3 lightDir;
smooth out vec4 pos;

layout (std140, binding = 4) uniform Data
{
    mat4 MVP;               //  64
    float heightScale;      //   4
    int invertHMap;         //   4
    vec3 ViewPosition;      //  16
    vec3 LightPosition;
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
};


uniform sampler2D hMap;

float height( vec2 tPos )
{
	tPos = tPos*10;
    float result;
    vec4 v = texelFetch(hMap, ivec2(tPos),0);
    if(v.b>0){

    	result = (255*(1-v.r) + (1-v.g) * 65280) / 65535;
    	result = -result;
    }
    else{
		result = (255*v.r + v.g * 65280) / 65535;
	}
	if(invertHMap==1) result = 1-result;

    return result;
}


void main () {
	if((gl_VertexID) % 2 == 1) pos = vec4(posGround.x, height(vec2(posGround.x,-posGround.y))*heightScale, posGround.y, 1.0);
	else pos = vec4(posGround.x, -20, posGround.y, 1.0);
    gl_Position = MVP * pos;

	cameraDir = normalize(ViewPosition-pos.xyz);
	lightDir = normalize(LightPosition.xyz - pos.xyz);
}