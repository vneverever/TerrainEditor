#version 430 core

layout (location = 0) in vec2 position;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec2 hMapC;

layout (std140, binding = 4) uniform Data
{
	mat4 MVP;
	float heightScale;
	int invertHMap;
	vec3 ViewPosition;
	vec3 LightPosition;
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
};


uniform sampler2D hMap;


uniform bool ShadowFlag;

smooth out vec4 pos;
smooth out vec3 eyePos;
smooth out vec3 worldNormal;
smooth out vec3 cameraDir;
smooth out vec3 lightDir;
smooth out float dark;
smooth out vec3 worldNormal2;
smooth out float occlusion;


float height( vec2 tPos )
{
	if(tPos.x <0) tPos.x = 0;
	else if(tPos.x > 1023) tPos.x = 1023;
	if(tPos.y < 0) tPos.y = 0;
	else if(tPos.y > 1023) tPos.y = 1023;

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

uniform sampler2D shadowUnit[10];

void main () {
	float h = height(hMapC.xy);
	float h2 = h;
	//if(h*heightScale<1){
	//	h2 = h2 + ((texture2D(shadowUnit[0], position).x*0.05));
	//}
	pos = vec4(position.x, h2 * heightScale, position.y,1.0);
	gl_Position = MVP * pos;

	vec3 off = vec3(1.0, 1.0, 0.0);

	float hL = height(hMapC.xy - off.xz);
	float hR = height(hMapC.xy + off.xz);
	float hD = height(hMapC.xy - off.zy);
	float hU = height(hMapC.xy + off.zy);

	float hLU = height(hMapC.xy + vec2(-1,1));
	float hRU = height(hMapC.xy + vec2(1,1));
	float hLD = height(hMapC.xy + vec2(-1,-1));
	float hRD = height(hMapC.xy + vec2(1,-1));

	float heights[9] = float[](
    height(hMapC.xy + vec2(-1,1)) , height(hMapC.xy + off.zy)                  , height(hMapC.xy + vec2(1,1)),
    height(hMapC.xy - off.xz)     , h 										   , height(hMapC.xy + off.xz),
    height(hMapC.xy + vec2(-1,-1)), height(hMapC.xy - off.zy)				   , height(hMapC.xy + vec2(1,-1))
);

	//occlusion = 1-((hLU+hRU+hLD+hRD+hL+hR+hU+hD) - (8*h));
	//occlusion = ((h)+1) / (((8+(hLU+hRU+hLD+hRD+hL+hR+hU+hD))/8));

	float kernel[9] = float[](
    1.0 / 2, 1.0 , 1.0 / 2,
    1.0  , 1		  , 1.0 ,
    1.0 / 2, 1.0  , 1.0 / 2
);

	for(int i = 0; i < 9; i++)
        occlusion += min(max((heights[i] * kernel[i] -h)*(heightScale/2),0),0.2);


	// occlusion = occlusion + max(0,hL  - h)*2;
	// occlusion = occlusion + max(0,hR  - h)*2;
	// occlusion = occlusion + max(0,hD  - h)*2;
	// occlusion = occlusion + max(0,hU  - h)*2;
	// occlusion = occlusion + max(0,hLU - h)*0.5;
	// occlusion = occlusion + max(0,hLD - h)*0.5;
	// occlusion = occlusion + max(0,hRU - h)*0.5;
	// occlusion = occlusion + max(0,hRD - h)*0.5;
	// occlusion = occlusion*100 * min(3,heightScale/20);
	//occlusion = min(max(occlusion,0.3),1);

	// worldNormal2.x = (hL - hR);
	// worldNormal2.z = -(hD - hU);
	// worldNormal2.y = 1.0;
	const float temp = 0.15/heightScale;
	vec3 va = vec3 (temp,hR-hL,0.0);
	vec3 vb = vec3 (0.0,hD-hU,temp);
	//worldNormal2 = normalize(cross(va,-vb));
	worldNormal2 = cross(normalize(vb),normalize(va));

	//worldNormal2 = normalize(vec3(hL-hR, 2 * heightScale/pos.y, hU-hD));
	worldNormal =  worldNormal2;
	/*
	dark = 0;
	if(false){
	vec3 gradient = normalize(pos.xyz - Light.position.xyz);

	if(dot(worldNormal,gradient)>0.6){
		dark = 1;
	}
	}
	*/
	cameraDir = normalize(ViewPosition-pos.xyz);
	lightDir = normalize(LightPosition.xyz - pos.xyz);
}