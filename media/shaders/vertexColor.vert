// Vertex Shader - file "vertexColor.vert"

#version 150 core

in  vec4 inPosition;
in	vec3 inColor;


uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;


out vec4 exColor;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * inPosition;
//	gl_Position = gl_Position + vec4(0, 0, -0.005, 0);
	exColor = vec4(inColor, 1);
}
