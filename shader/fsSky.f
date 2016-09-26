#version 430 core
uniform samplerCube skyUnit;

smooth in vec3 eyeDirection;

layout (location = 0 ) out vec4 colorOut;

void main() {
	//gl_FragDepth = 0.99999;
	const float gamma = 2.2;
    vec3 mapped = texture(skyUnit, eyeDirection).xyz;
	mapped = pow(mapped,vec3(1.0/gamma));
	colorOut = vec4(mapped,1.0);
}