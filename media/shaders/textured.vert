// Vertex Shader - file "textured.vert"

#version 150 core

in  vec4 inPosition;
in	vec2 inUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 exUV;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * inPosition;
	exUV = inUV;
}
