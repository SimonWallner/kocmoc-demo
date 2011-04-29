// Vertex Shader - file "base.vert"

#version 150 core

in  vec4 inPosition;
in	vec2 inTexCoord0;
in  vec3 inNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 worldSpacePosition;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * inPosition;
	worldSpacePosition = (modelMatrix * inPosition).xyz;
}


