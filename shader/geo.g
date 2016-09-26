#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 600) out;

void main() {

    for(int i=0; i<100;i++){
    	gl_Position = gl_in[0].gl_Position + vec4(i*10, i*10,i*10,1.0);
    	EmitVertex();
    }

    EndPrimitive();
}