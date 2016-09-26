#version 430 core
layout (points) in;
//layout (triangle_strip, max_vertices = 40) out;
layout (line_strip, max_vertices = 200) out;

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
uniform vec4 hit;

void emitCube(vec3 pos, float size){
	gl_Position = MVP * vec4((pos + vec3(-size,-size,-size)),1.0);
	EmitVertex();
	gl_Position = MVP * vec4((pos + vec3(size,-size,-size) ),1.0);
	EmitVertex();
	gl_Position = MVP * vec4((pos + vec3(size,size,-size)  ),1.0);
	EmitVertex();
	EndPrimitive();
	gl_Position = MVP * vec4((pos + vec3(size,size,-size)  ),1.0);
	EmitVertex();
	gl_Position = MVP * vec4((pos + vec3(-size,size,-size) ),1.0);
	EmitVertex();
	gl_Position = MVP * vec4((pos + vec3(-size,-size,-size)),1.0);
	EmitVertex();
	EndPrimitive();
	//gl_Position = MVP * vec4((pos + vec3(-size,size,-size) ),1.0);
	//EmitVertex();
	//gl_Position = MVP * vec4((pos + vec3(-size,size,-size) ),1.0);
	//EmitVertex();
}

void shootRay(vec3 pos){
	gl_Position = MVP * vec4(pos,1.0);
	EmitVertex();
}

void emitHit(){
	float size = 0.3f;
	vec3 s = vec3(size, -size, 0);
	gl_Position = MVP * vec4(hit.xyz + s.yyy,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.xyy,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.xyx,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.yyx,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.yyy,1.0f);
	EmitVertex();

	gl_Position = MVP * vec4(hit.xyz + s.yxy,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.xxy,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.xxx,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.yxx,1.0f);
	EmitVertex();
	gl_Position = MVP * vec4(hit.xyz + s.yxy,1.0f);
	EmitVertex();
}

void main() {
	emitHit();
    //for(int i=0; i<100;i++){
    	//gl_Position = gl_in[0].gl_Position + vec4(i*0, i*100,i*100,1.0);
    // 	//EmitVertex();
    // 	gl_Position = vec4(0,100,0,1);
    // 	EmitVertex();
    // 	gl_Position = vec4(0,100,100,1);
    // 	EmitVertex();
    // 	gl_Position = vec4(100,100,100,1);
    // 	EmitVertex();
    // 	EndPrimitive();
    // 	gl_Position = vec4(0,100,0,1);
    // 	EmitVertex();
    // 	gl_Position = vec4(100,100,100,1);
    // 	EmitVertex();
    // 	gl_Position = vec4(100,100,0,1);
    // 	EmitVertex();
    // 	EndPrimitive();
    // //}
    //for(int i=0;i<200;i+=10){
    	//emitCube(vec3(100+ i*start.x,20 + i * start.y,-100 +  i * start.z), 50);
    	//shootRay(start.xyz - i*normalize(ViewPos));
    	//shootRay(vec3(ViewPos.x, ViewPos.y, ViewPos.z) + i * vec3(start.x, start.y, start.z);
    	//shootRay(vec3(ViewPos.x, ViewPos.y, ViewPos.z) + i * vec3(1.0, 0, 1.0));
    	//shootRay(ViewPos - i *ViewPos);
    	//emitCube(ViewPosition + i*start.xyz, 3);
    //}
 //    gl_Position = MVP * (start + vec4(0, 20, 0, 1));
 //    EmitVertex();
	// gl_Position = MVP * (start + vec4(102.3, 20, 0, 1));
	// EmitVertex();
	// gl_Position = MVP * (start + vec4(102.3, 20, -102.3, 1));
	// EmitVertex();

	// gl_Position = MVP * (start + vec4(102.3, 20.f, -102.3, 1));
	// EmitVertex();
	// gl_Position = MVP * (start + vec4(0, 20, -102.3, 1));
	// EmitVertex();
	// gl_Position = MVP * (start + vec4(0, 20, 0, 1));
	// EmitVertex();
	// EndPrimitive();


}