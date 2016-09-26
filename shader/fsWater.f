#version 430 core
layout (location = 0 ) out vec4 colorOut;

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

smooth in vec3 cameraDir;
smooth in vec3 lightDir;
smooth in vec4 pos;

uniform sampler2D water;
uniform sampler2D hMap;
uniform samplerCube sky;

uniform float timer;
uniform float timer2;

float height( vec2 tPos )
{
	//if(tPos.x<0 || tPos.y <0 || tPos.x > 51.1 || tPos.y > 51.1) return 0;
	vec2 v = texture(hMap,tPos,0).yz;
	float result = (255*v.y + v.x * 65280) / 65535;
	
	if(invertHMap==1) result = 1-result;
	
	//result = result;
    return result;
}
void main()
{
	float heightGround = height(vec2(pos.x/102.3,pos.z/-102.3))*heightScale;
	
	if(pos.y < heightGround) discard;
	float distanceToGround = (pos.y - heightGround);
	float water=min(1.3, distanceToGround/2);
	
	vec3 normalDirection = mix(vec3 (1.0,0,0)*sin(-timer2*4+pos.x/5-pos.z)*pow(sin(timer2*2+pos.x/5-pos.z),2), vec3 (-1.0,0,0)*sin(timer2+pos.x-pos.z*2),(timer*2-0.3)*5);
	vec3 normal2 = mix(vec3 (0,0,1)*pow(sin(timer2*4+pos.z*2 - 2*pos.x),2), vec3 (0.0,0,-1)*pow(sin(3*timer2+pos.z/2 +pos.z/10),2),(timer*(2)-0.3)*5);
	normalDirection = normalize(-normalDirection + -normal2 + vec3(0,6,0));
	//vec3 textureColor = texture(water, texC/10,3).xyz;
	//vec3 cameraDir = normalize(cameraDir);
	//vec3 lightDir = normalize (lightDir);
	
	vec3 cameraDir = normalize(ViewPosition-pos.xyz);
	vec3 lightDir = normalize(LightPosition.xyz - pos.xyz);
	
	vec3 H = normalize(lightDir + cameraDir);
	if(distanceToGround< heightScale/255 * 3){
		normalDirection = normalDirection + vec3(0,0.2,0);
	
		float diffuseTerm = dot(lightDir, normalDirection);
		vec3 difLight = vec3(1,1,1) * diffuseTerm; 
	
		float specularTerm = 0;
		vec3 specLight;
	
		specularTerm = pow(max(dot(normalDirection, H), 0), 50);	
		specLight = vec3(2,2,2) * specularTerm;
	
	
		float alpha = distanceToGround / (heightScale/255 * 3);
		colorOut = vec4((difLight+specLight) * mix(vec4(0.8,0.8,0.8,0.1).xyz, vec4(0,0.7,1.0,0.3).xyz, alpha+0.7), mix(0.2,0.8,alpha));
	}else{
	
		//normalDirection= vec3(0,1,0);
		

		
		float diffuseTerm = dot(lightDir, normalDirection);
		vec3 difLight = vec3(1,1,1) * diffuseTerm; 
		
		
		vec3 ref = reflect(-normalize(cameraDir), normalDirection);
		vec4 invertedFresnel = vec4( dot(normalDirection, H ) );
        vec4 fresnelTerm = 1.0 - invertedFresnel;
		
		vec3 refColor = texture(sky,ref).xyz * fresnelTerm.xyz;
		
		float specularTerm = 0;
		vec3 specLight;
		
		
		specularTerm = pow(max(dot(normalDirection, H), 0), 80);	
		specLight = vec3(1,1,1) * specularTerm;
	
		colorOut =  vec4((difLight+specLight+0.2) * mix(vec4(0.0,0.0,0.5,1),vec4(0.0,0.5,1,1),1-water+0.5).xyz,max(min(water*0.95+0.5,0.95),0.42));
		colorOut = mix (vec4(refColor*2,1), colorOut,1.0);
		
	}
	
	//colorOut = vec4(normalDirection,1);
}