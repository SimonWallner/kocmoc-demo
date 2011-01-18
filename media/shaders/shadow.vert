// Vertex Shader – file "shadow.vert"

#version 150 core

in  vec4 inPosition;

uniform mat4 modelMatrix;
uniform mat4 shadowPV;

void main(void)
{
	gl_Position = shadowPV * modelMatrix * inPosition;
}

