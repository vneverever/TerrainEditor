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

uniform sampler2D hMap;
uniform vec3 texLevel[10];

//BOOLS
uniform bool LightFlag;
uniform bool SpecularFlag;
uniform bool NormalMapFlag;
uniform bool ShadowFlag;
uniform bool TextureFlag;
uniform float TextureScale;

smooth in vec4 pos;
smooth in vec3 worldNormal;
smooth in vec3 worldNormal2;
smooth in vec3 cameraDir;
smooth in vec3 lightDir;
smooth in float dark;
smooth in float occlusion;

//uniform float waterLevel;

float height( vec2 tPos )
{
	//if(tPos.x<0 || tPos.y <0 || tPos.x > 51.1 || tPos.y > 51.1) return 0;
	vec2 v = texture(hMap,tPos/102.3,0).yz;
	float result = (255*v.y + v.x * 65280) / 65535;

	if(invertHMap==1) result = 1-result;

	//result = result;
    return result;
}

struct LightInfo {
	vec4 position; // Light position in eye coords.
	vec4 Ld; // Diffuse light intensity
	vec4 Ls; // Specular light intensity
};
uniform LightInfo Light;

uniform vec4 sceneAmbient;

struct MaterialInfo {
	  vec4 Ka; // Ambient reflectivity
	  vec4 Kd; // Diffuse reflectivity
	  vec4 Ks; // Specular reflectivity
	  float Shininess; // Specular shininess factor
};
uniform MaterialInfo Material;

uniform sampler2D texUnit[10];
uniform sampler2D normalUnit[10];
uniform sampler2D shadowUnit[10];

vec4 texColor(int i, vec2 pos, vec2 texDx, vec2 texDy){
	vec4 color;

	if(i==0) color = textureGrad(texUnit[0], pos, texDx, texDy);
	else if(i==1) color = textureGrad(texUnit[1], pos, texDx, texDy);
	else if(i==2) color = textureGrad(texUnit[2], pos, texDx, texDy);
	else if(i==3) color = textureGrad(texUnit[3], pos, texDx, texDy);
	else if(i==4) color = textureGrad(texUnit[4], pos, texDx, texDy);
	else if(i==5) color = textureGrad(texUnit[5], pos, texDx, texDy);
	else if(i==6) color = textureGrad(texUnit[6], pos, texDx, texDy);
	else if(i==7) color = textureGrad(texUnit[7], pos, texDx, texDy);
	else if(i==8) color = textureGrad(texUnit[8], pos, texDx, texDy);
	else if(i==9) color = textureGrad(texUnit[9], pos, texDx, texDy);
	return color;
}

vec4 normalColor(int i, vec2 pos, vec2 texDx, vec2 texDy){
	vec4 color;
	if(i==0) color = textureGrad(normalUnit[0], pos, texDx, texDy);
	else if(i==1) color = textureGrad(normalUnit[1], pos, texDx, texDy);
	else if(i==2) color = textureGrad(normalUnit[2], pos, texDx, texDy);
	else if(i==3) color = textureGrad(normalUnit[3], pos, texDx, texDy);
	else if(i==4) color = textureGrad(normalUnit[4], pos, texDx, texDy);
	else if(i==5) color = textureGrad(normalUnit[5], pos, texDx, texDy);
	else if(i==6) color = textureGrad(normalUnit[6], pos, texDx, texDy);
	else if(i==7) color = textureGrad(normalUnit[7], pos, texDx, texDy);
	else if(i==8) color = textureGrad(normalUnit[8], pos, texDx, texDy);
	else if(i==9) color = textureGrad(normalUnit[9], pos, texDx, texDy);
	return color;
}

vec4 specColor(int i, vec2 pos, vec2 texDx, vec2 texDy){
	vec4 color;
	
	if(i==0) color = textureGrad(shadowUnit[0], pos, texDx, texDy);
	else if(i==1) color = textureGrad(shadowUnit[1], pos, texDx, texDy);
	else if(i==2) color = textureGrad(shadowUnit[2], pos, texDx, texDy);
	else if(i==3) color = textureGrad(shadowUnit[3], pos, texDx, texDy);
	else if(i==4) color = textureGrad(shadowUnit[4], pos, texDx, texDy);
	else if(i==5) color = textureGrad(shadowUnit[5], pos, texDx, texDy);
	else if(i==6) color = textureGrad(shadowUnit[6], pos, texDx, texDy);
	else if(i==7) color = textureGrad(shadowUnit[7], pos, texDx, texDy);
	else if(i==8) color = textureGrad(shadowUnit[8], pos, texDx, texDy);
	else if(i==9) color = textureGrad(shadowUnit[9], pos, texDx, texDy);
	return color;
}
layout (location = 0 ) out vec4 colorOut;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


vec3 blend_rnm(vec4 n1, vec4 n2)
{
    //n1 = n1*2-1;
    //n2 = n2*2-1;
    vec3 r = vec3(n1.xy + n2.xy, n1.z*n2.z);
    return normalize(r);
}

vec3 blend_linear(vec4 n1, vec4 n2)
{

    vec3 r = vec3(n1 + n2)*2 - 2;
    return normalize(r);
}


vec3 blend_overlay(vec4 n1, vec4 n2)
{
    vec3 t = n1.xyz*vec3( 2,  2, 2) + vec3(-1, -1,  0);
    vec3 u = n2.xyz*vec3(-2, -2, 2) + vec3( 1,  1, -1);
    vec3 r = t*dot(t, u) - u*t.z;
    return normalize(r);
}

void main()
{
	vec3 textureColor;
	vec3 textureColor2;
	vec3 wNormal = normalize(worldNormal2);
	vec3 wNormal2 = normalize(worldNormal);

	float tighten = 0.4679f;
	float mXY = max(abs(wNormal2.z)-tighten,0.0000);
	float mXZ = max(abs(wNormal2.y)-tighten,0.0000);
	float mYZ = max(abs(wNormal2.x)-tighten,0.0000);
	float total = mXY + mXZ + mYZ;



	mXY /= total;
	mXZ /= total;
	mYZ /= total;

	vec3 shadowed = vec3(1,0,0);
	if(ShadowFlag==true){

	vec3 gradient = normalize(pos.xyz - LightPosition.xyz);

	if(dot(wNormal,gradient)>0.6){
		shadowed.x = 0;
	}
	if(shadowed.x>0.1){
	float dist = distance(LightPosition.xyz, pos.xyz);
	float heightFromMap = 0.0;

	gradient *= 0.5;
	int counter = 0;
	vec3 currentPoint = pos.xyz - gradient*0.2;

		while(currentPoint.x>=0 && currentPoint.x<=102.3 && currentPoint.z>=-102.3 && currentPoint.z<=0 && currentPoint.y<=heightScale){
			heightFromMap = height(vec2(currentPoint.x,-currentPoint.z))*heightScale;
			//float temp = heightFromMap;
			if(currentPoint.y<heightFromMap){


				shadowed.y=1;
				currentPoint += 0.9*gradient;
				heightFromMap = height(vec2(currentPoint.x,-currentPoint.z))*heightScale;
				if(currentPoint.y<heightFromMap){
				dist = distance(currentPoint,pos.xyz)/dist;
				//dist=temp-currentPoint.y;
				shadowed.x = min((dist/0.35)-0.4,1);//min((i*0.5)/dist,0.9);1-min(dist-0.3,1);

				break;
				}
			}
			currentPoint -= gradient;
		}
	}
	}
	else{
		float dark=0;
		shadowed.x=1;
	}
	mat3 TBN;
	vec3 tx,ty,tz;
	vec3 bx, by, bz;

	vec3 tangent, binormal;


	vec4 cXY, cXY2;
	vec4 cXZ, cXZ2;
	vec4 cYZ, cYZ2;

	vec4 cXYMix, cXY2Mix;
	vec4 cXZMix, cXZ2Mix;
	vec4 cYZMix, cYZ2Mix;
	
	vec4 nXY, nXY2;
	vec4 nXZ, nXZ2;
	vec4 nYZ, nYZ2;
	
	vec4 sXY, sXY2;
	vec4 sXZ, sXZ2;
	vec4 sYZ, sYZ2;
	
	
	
	float r = 0;//rand(vec2(pos.x,0))*0.5; 
	//r = max(r-0.18,0.0);
	//r = 0;
	float scale = TextureScale;
	vec4 normalTexture;
	vec4 normalTexture2;
	vec3 mixedNormal;
	
	vec3 spec  = vec3(Material.Ks);
	vec3 spec2;
	
	float alpha1 = 0;
	float alpha2 = 0;
	
	vec2 posXY = pos.xy * scale;
	vec2 posXZ = pos.xz * scale;
	vec2 posYZ = pos.yz * scale;
	
	bool textureEnabled = TextureFlag;
	bool normalMapEnabled = NormalMapFlag;
	bool specularMapEnabled = SpecularFlag;
	bool perturbedNormals = NormalMapFlag;
	
	int count = 0;
	
	vec2 texDxXY = dFdx(posXY);
	vec2 texDyXY = dFdy(posXY);
	vec2 texDxXZ = dFdx(posXZ);
	vec2 texDyXZ = dFdy(posXZ);
	vec2 texDxYZ = dFdx(posYZ);
	vec2 texDyYZ = dFdy(posYZ);


	if (pos.y >= 18.0 && count<2)	{
		count++;
		int a=int(texLevel[9].x);
		int b=int(texLevel[9].y);
		int c=int(texLevel[9].z);


		alpha1 = pos.y - 18;
		alpha1 = alpha1*3;

		if (alpha1 > 1) alpha1 = 1;

		if(alpha1 <= 0){
			alpha1 = 0;
		}else{
		if(textureEnabled){
		cXY = texColor(a, posXY, texDxXY, texDyXY);
		cXZ = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY = specColor(a, posXY, texDxXY, texDyXY);
		sXZ = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 16.0 && count<2)	{
		count++;
		if(alpha1<1){
		int a=int(texLevel[8].x);
		int b=int(texLevel[8].y);
		int c=int(texLevel[8].z);


		if(textureEnabled){
		cXY2 = texColor(a, posXY, texDxXY, texDyXY);
		cXZ2 = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ2 = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY2 = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ2 = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ2 = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY2 = specColor(a, posXY, texDxXY, texDyXY);
		sXZ2 = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ2 = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 14.0 && count<2)	{
		count++;
		int a=int(texLevel[7].x);
		int b=int(texLevel[7].y);
		int c=int(texLevel[7].z);
		
		if(pos.y>=16){
			alpha1 = 16.34 - pos.y;
		}else{		
			alpha1 = pos.y - 14;
		}
		
		alpha1 = alpha1*3;
		if (alpha1 > 1) alpha1 = 1;
		
		if(alpha1 <= 0){
			alpha1 = 0;
		}else{
		if(textureEnabled){
		cXY = texColor(a, posXY, texDxXY, texDyXY);
		cXZ = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY = specColor(a, posXY, texDxXY, texDyXY);
		sXZ = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 12.0 && count<2)	{
		count++;
		if(alpha1<1){

		int a=int(texLevel[6].x);
		int b=int(texLevel[6].y);
		int c=int(texLevel[6].z);

		if(textureEnabled){
		cXY2 = texColor(a, posXY, texDxXY, texDyXY);
		cXZ2 = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ2 = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY2 = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ2 = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ2 = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY2 = specColor(a, posXY, texDxXY, texDyXY);
		sXZ2 = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ2 = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 10.0 && count<2)	{
		count++;
		int a=int(texLevel[5].x);
		int b=int(texLevel[5].y);
		int c=int(texLevel[5].z);

		if(pos.y>=12){
			alpha1 = 12.34 - pos.y;
		}else{
			alpha1 = pos.y - 10;
		}

		alpha1 = alpha1*3;
		if (alpha1 > 1) alpha1 = 1;

		if(alpha1 <= 0){
			alpha1 = 0;
		}else{
		if(textureEnabled){
		cXY = texColor(a, posXY, texDxXY, texDyXY);
		cXZ = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY = specColor(a, posXY, texDxXY, texDyXY);
		sXZ = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 8.0 && count<2)	{
		count++;
		if(alpha1<1){
		int a=int(texLevel[4].x);
		int b=int(texLevel[4].y);
		int c=int(texLevel[4].z);

		if(textureEnabled){
		cXY2 = texColor(a, posXY, texDxXY, texDyXY);
		cXZ2 = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ2 = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY2 = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ2 = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ2 = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY2 = specColor(a, posXY, texDxXY, texDyXY);
		sXZ2 = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ2 = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 6.0 && count<2)	{
		count++;
		int a=int(texLevel[3].x);
		int b=int(texLevel[3].y);
		int c=int(texLevel[3].z);

		if(pos.y>=8){
			alpha1 = 8.34 - pos.y;
		}else{
			alpha1 = pos.y - 6;
		}

		alpha1 = alpha1*3;
		if (alpha1 > 1) alpha1 = 1;

		if(alpha1 <= 0){
			alpha1 = 0;
		}else{
		if(textureEnabled){
		cXY = texColor(a, posXY, texDxXY, texDyXY);
		cXZ = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY = specColor(a, posXY, texDxXY, texDyXY);
		sXZ = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 4.0 && count<2)	{
		count++;
		if(alpha1<1){
		int a=int(texLevel[2].x);
		int b=int(texLevel[2].y);
		int c=int(texLevel[2].z);
		
		if(textureEnabled){
		cXY2 = texColor(a, posXY, texDxXY, texDyXY);
		cXZ2 = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ2 = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY2 = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ2 = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ2 = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY2 = specColor(a, posXY, texDxXY, texDyXY);
		sXZ2 = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ2 = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	}if (pos.y >= 2.0 && count<2)	{
		count++;
		int a=int(texLevel[1].x);
		int b=int(texLevel[1].y);
		int c=int(texLevel[1].z);
	
		if(pos.y>=4){
			alpha1 = 4.34 - pos.y;
		}else{		
			alpha1 = pos.y - 2;
		}
		
		alpha1 = alpha1*3;
		if (alpha1 > 1) alpha1 = 1;
		
		if(alpha1 < 0){
			alpha1 = 0;
		}
		if(textureEnabled){
		cXY = texColor(a, posXY, texDxXY, texDyXY);
		cXZ = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY = specColor(a, posXY, texDxXY, texDyXY);
		sXZ = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
	}if (count<2)	{
		count++;
		if(alpha1<1){
		int a=int(texLevel[0].x);
		int b=int(texLevel[0].y);
		int c=int(texLevel[0].z);
		
		if(textureEnabled){
		cXY2 = texColor(a, posXY, texDxXY, texDyXY);
		cXZ2 = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ2 = texColor(c, posYZ, texDxYZ, texDyYZ);
		
		cXY2Mix = texColor(a, posXY, texDxXY, texDyXY);
		cXZ2Mix = texColor(b, posXZ, texDxXZ, texDyXZ);
		cYZ2Mix = texColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(normalMapEnabled){
		nXY2 = normalColor(a, posXY, texDxXY, texDyXY);
		nXZ2 = normalColor(b, posXZ, texDxXZ, texDyXZ);
		nYZ2 = normalColor(c, posYZ, texDxYZ, texDyYZ);
		}
		if(specularMapEnabled){
		sXY2 = specColor(a, posXY, texDxXY, texDyXY);
		sXZ2 = specColor(b, posXZ, texDxXZ, texDyXZ);
		sYZ2 = specColor(c, posYZ, texDxYZ, texDyYZ);
		}
		}
	} 
	if(alpha1>1) alpha1=1;
	if(alpha1<0) alpha1=0;
	if(textureEnabled){
		textureColor = vec3(cXY*mXY + cXZ*mXZ + cYZ*mYZ);
		textureColor2 = vec3(cXY2*mXY + cXZ2*mXZ + cYZ2*mYZ);
		//textureColor2 = 0.5*textureColor2 + vec3(cXY2Mix*mXY + cXZ2Mix*mXZ + cYZ2Mix*mYZ)*0.5;
		if(alpha1==1){
			textureColor = textureColor;
		}else if(alpha1==0){
			textureColor = textureColor2;
		}else{
			textureColor = textureColor*alpha1 + textureColor2*(1-alpha1);
		}
	}else{
		textureColor = vec3(0.5,0.5,0.5);
	}
	if(LightFlag){
	
	if(specularMapEnabled){
		spec = vec3(sXY*mXY + sXZ*mXZ + sYZ*mYZ);
		spec2 = vec3(sXY2*mXY + sXZ2*mXZ + sYZ2*mYZ);
		if(alpha1==1){
			spec = spec;
		}else if(alpha1==0){
			spec = spec2;
		}else{
			spec = spec*alpha1 + spec2*(1-alpha1);
		}
	}else{
		spec = vec3(Material.Ks);
	}


	if(dark>0.1){
		colorOut = vec4(0,0,0,1);
		return;
	}else{

		if(normalMapEnabled){
		tz = vec3(1,0,0);
		bz = vec3(0,1,0);

		ty = vec3(1,0,0);
		by = vec3(0,0,1);

		tx = vec3(0,1,0);
		bx = vec3(0,0,1);

		tangent = normalize(tx * mYZ + ty * mXZ + tz * mXY);

		binormal = bx * mYZ + by * mXZ + bz * mXY;

		tangent = normalize(tangent - dot(tangent, wNormal) * wNormal);
		binormal = normalize(cross(tangent,wNormal));
		TBN = mat3(tangent, binormal, wNormal);


			if(alpha1>0 && alpha1<1){
				normalTexture = ((nXY*mXY + nXZ*mXZ + nYZ*mYZ)*2-1);
				normalTexture2 = ((nXY2*mXY + nXZ2*mXZ + nYZ2*mYZ)*2-1);
				normalTexture = vec4(normalTexture.xy*alpha1,normalTexture.zw);
				normalTexture2 = vec4(normalTexture2.xy*(1-alpha1),normalTexture2.zw);
				mixedNormal = blend_rnm(normalTexture, normalTexture2);
			}else if(alpha1 == 1.0){
				normalTexture = ((nXY*mXY + nXZ*mXZ + nYZ*mYZ)*2-1);
				mixedNormal = vec3(normalTexture.xy,normalTexture.z);
			}else{
				normalTexture2 = ((nXY2*mXY + nXZ2*mXZ + nYZ2*mYZ)*2-1);
				mixedNormal = vec3(normalTexture2.xy,normalTexture2.z);
			}
			mixedNormal = normalize(mixedNormal);

		}


		if(perturbedNormals){

		mixedNormal = TBN*mixedNormal;//wNormal +  tangent*normalTexture.x + binormal * normalTexture.y;
		}else{
			mixedNormal = wNormal;
		}
		vec3 normalDirection = normalize(mixedNormal);
		//cameraDir = normalize(ViewPosition-pos.xyz);
		//lightDir = normalize(LightPosition.xyz - pos.xyz);
		vec3 cameraDir = normalize(cameraDir);
		vec3 lightDir = normalize(lightDir);

		vec3 ambLight, difLight, specLight;
		//ambient
		//ambLight = max(sceneAmbient.xyz - occlusion,0.0);
		ambLight = (sceneAmbient.xyz-max((1 - exp(-occlusion*3)),0.03));

		//diffuse
		float diffuseTerm = max(dot(lightDir, normalDirection), 0.0);
		difLight = Material.Kd.xyz * Light.Ld.xyz * diffuseTerm;

		//specular



		if(shadowed.x >=1 || shadowed.y!=1){

			if(dot(normalDirection, lightDir)>0.0){
				float specularTerm = 1;
				vec3 H = normalize(lightDir + cameraDir);

				specularTerm = pow(max(dot(normalDirection, H), 0), Material.Shininess);
				specLight = spec * Light.Ls.xyz * specularTerm;
				colorOut = vec4(textureColor * (ambLight + difLight + specLight), 1);
				//colorOut = vec4(textureColor * (ambLight), 1);


			}else{
				colorOut = vec4(textureColor * (ambLight + difLight), 1);
				//colorOut = vec4(textureColor * (ambLight), 1);
			}
			 //colorOut = vec4(alpha1,0,0,1);
		}
		else{
			colorOut = vec4(textureColor * (ambLight +difLight), 1);
			colorOut = min(vec4(colorOut.xyz* shadowed.x,1),colorOut);

		}
	}
	}else{
		colorOut = vec4(textureColor,1);
		//colorOut = vec4(((texture2D(hMap,vec2(pos.x/102.3,-pos.z/102.3),-1).xyz)),1);
	}
	const float gamma = 1.8;
	const float exposure = 1.0;
	//vec3 mapped = colorOut.xyz/(colorOut.xyz + vec3(1.0));

	vec3 mapped = vec3(1.0) - exp(-colorOut.xyz*exposure);
	//mapped = pow(mapped,vec3(1.0/gamma));
	colorOut = vec4(mapped,1.0);

}