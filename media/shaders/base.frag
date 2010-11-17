// Fragment Shader – file "base.frag"

#version 150 core

in vec4 fragmentPos;
in vec2 texCoord0;
in vec3 fragmentNormal;
in vec3 lightDirection;
in vec3 incidentLightPosition;


out vec4 fragmentColor;

void main(void)
{
	fragmentColor = vec4(ex_color,1.0);
}
