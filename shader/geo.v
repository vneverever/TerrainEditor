#version 330 core
layout (location = 0) in vec3 position;

//out VS_OUT {
//    vec3 start;
//} vs_out;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
    //vs_out.start = normalize(vec3(projection * vec4(normalMatrix * normal, 1.0)));
}