#version 330
in vec3 LightIntensity;
in vec2 texc;
layout( location = 0 ) out vec4 FragColor;
uniform sampler2D texUnit;

void main() {
		vec4 texColor = texture2D(texUnit, texc);
		FragColor = vec4(LightIntensity, 1.0);
}