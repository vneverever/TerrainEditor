#version 430
 
smooth in vec4 pos;
smooth in vec3 normalDirection;
smooth in vec2 texC;
smooth in vec3 worldNormal;
smooth in vec4 eyeVec;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;
uniform mat4 ViewInverse;
uniform float heightScale;
 
struct LightInfo {
	vec4 position; // Light position in eye coords.
	vec4 Ld; // Diffuse light intensity
	vec4 Ls; // Specular light intensity
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

uniform vec4 sceneAmbient;
uniform LightInfo Light;

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

layout (location = 0 ) out vec4 colorOut;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}
 
vec3 perturb_normal( vec3 N, vec3 V, vec3 map, vec2 texcoord)
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
   map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}


void main()
{
		vec3 textureColor;
	vec3 textureColor2;
	
	vec3 wNormal = normalize(worldNormal);
	float tighten = 0.4679f;
	
	float mXY = max(abs(wNormal.z)-tighten,0.0);
	float mXZ = max(abs(wNormal.y)-tighten,0.0);
	float mYZ = max(abs(wNormal.x)-tighten,0.0);
	float total = mXY + mXZ + mYZ;
	
	mXY /= total;
	mXZ /= total;
	mYZ /= total;
	vec4 cXY;
	vec4 cXZ;
	vec4 cYZ;
	
	vec4 nXY;
	vec4 nXZ;
	vec4 nYZ;
	
	vec4 sXY;
	vec4 sXZ;
	vec4 sYZ;
	
	
	
	float r = 0;//rand(vec2(pos.x,0))*0.5; 
	//r = max(r-0.18,0.0);
	//r = 0;
	float scale = 0.5;
	vec3 normalTexture;
	
	vec3 spec  = vec3(Material.Ks);
	vec3 diffu = vec3(Material.Kd);
	
	if (pos.y+r >= 9.5)	{
		int a=1;
		int b=0;
		int c=1;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 8.0)	{
		int a=1;
		int b=0;
		int c=3;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 7.0)	{
		int a=3;
		int b=0;
		int c=3;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 5.5)	{
		int a=3;
		int b=3;
		int c=3;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 5.0)	{
		int a=3;
		int b=4;
		int c=3;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 4.5)	{
		int a=4;
		int b=4;
		int c=3;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 4.0)	{
		int a=4;
		int b=4;
		int c=4;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 3.0)	{
		int a=4;
		int b=5;
		int c=4;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 2.0)	{
		int a=4;
		int b=5;
		int c=5;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}else if (pos.y+r >= 1.0)	{
		int a=5;
		int b=5;
		int c=6;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}
	else{
		int a=6;
		int b=9;
		int c=8;
		cXY = texture2D(texUnit[a], pos.xy*scale);
		cXZ = texture2D(texUnit[b], pos.xz*scale);
		cYZ = texture2D(texUnit[c], pos.yz*scale);
		nXY = texture2D(normalUnit[a], pos.xy*scale);
		nXZ = texture2D(normalUnit[b], pos.xz*scale);
		nYZ = texture2D(normalUnit[c], pos.yz*scale);
		sXY = texture2D(shadowUnit[a], pos.xy*scale);
		sXZ = texture2D(shadowUnit[b], pos.xz*scale);
		sYZ = texture2D(shadowUnit[c], pos.yz*scale);
	}
	
	spec = vec3(sXY*mXY + sXZ*mXZ + sYZ*mYZ);
	textureColor = vec3(cXY*mXY + cXZ*mXZ + cYZ*mYZ);
	normalTexture = vec3(nXY*mXY + nXZ*mXZ + nYZ*mYZ);
	
	vec4 lightDir = Light.position + eyeVec;
	vec2 uv = pos.xz*scale;
	
	vec3 N = normalize(normalDirection.xyz);
	vec3 L = normalize(lightDir.xyz);
	vec3 V = normalize(eyeVec.xyz);
	vec3 PN = perturb_normal(N, V, normalTexture, pos.xz*scale);
	
	vec4 final_color = vec4(textureColor,1.0); 
  PN = normalize(N + PN);
  float lambertTerm = dot(PN, L);
  if (lambertTerm > 0.0)
  {
    final_color += Light.Ls * vec4(diffu,1.0) * lambertTerm * vec4(textureColor,1.0);  
    
    vec3 E = normalize(eyeVec.xyz);
    vec3 R = reflect(-L, PN);
    float specular = pow( max(dot(R, E), 0.0), Material.Shininess);
    final_color += Light.Ls * vec4(spec,1.0) * specular;  
  }
	
	
	colorOut = final_color;
	
}